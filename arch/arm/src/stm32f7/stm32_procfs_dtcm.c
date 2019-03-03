
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>

#include <sys/types.h>
#include <sys/statfs.h>
#include <sys/stat.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <ecr/arch.h>
#include <ecr/sched.h>
#include <ecr/kmalloc.h>
#include <ecr/fs/fs.h>
#include <ecr/fs/procfs.h>
#include <ecr/fs/dirent.h>

#include <arch/irq.h>

#include "stm32_dtcm.h"

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_FS_PROCFS)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DTCM_LINELEN     64

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This enumeration identifies all of the thread attributes that can be
 * accessed via the procfs file system.
 */

/* This structure describes one open "file" */

struct dtcm_file_s
{
  struct procfs_file_s  base;    /* Base open file structure */
  unsigned int linesize;         /* Number of valid characters in line[] */
  char line[DTCM_LINELEN];       /* Pre-allocated buffer for formatted lines */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* File system methods */

static int     dtcm_open(FAR struct file *filep, FAR const char *relpath,
                         int oflags, mode_t mode);
static int     dtcm_close(FAR struct file *filep);
static ssize_t dtcm_read(FAR struct file *filep, FAR char *buffer,
                         size_t buflen);
static int     dtcm_dup(FAR const struct file *oldp,
                        FAR struct file *newp);
static int     dtcm_stat(FAR const char *relpath, FAR struct stat *buf);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* See include/nutts/fs/procfs.h
 * We use the old-fashioned kind of initializers so that this will compile
 * with any compiler.
 */

const struct procfs_operations dtcm_procfsoperations =
{
  dtcm_open,       /* open */
  dtcm_close,      /* close */
  dtcm_read,       /* read */
  NULL,            /* write */
  dtcm_dup,        /* dup */
  NULL,            /* opendir */
  NULL,            /* closedir */
  NULL,            /* readdir */
  NULL,            /* rewinddir */
  dtcm_stat        /* stat */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dtcm_open
 ****************************************************************************/

static int dtcm_open(FAR struct file *filep, FAR const char *relpath,
                     int oflags, mode_t mode)
{
  FAR struct dtcm_file_s *priv;

  finfo("Open '%s'\n", relpath);

  /* PROCFS is read-only.  Any attempt to open with any kind of write
   * access is not permitted.
   *
   * REVISIT:  Write-able proc files could be quite useful.
   */

  if ((oflags & O_WRONLY) != 0 || (oflags & O_RDONLY) == 0)
    {
      ferr("ERROR: Only O_RDONLY supported\n");
      return -EACCES;
    }

  /* "cpuload" is the only acceptable value for the relpath */

  if (strcmp(relpath, "dtcm") != 0)
    {
      ferr("ERROR: relpath is '%s'\n", relpath);
      return -ENOENT;
    }

  /* Allocate a container to hold the task and attribute selection */

  priv = (FAR struct dtcm_file_s *)kmm_zalloc(sizeof(struct dtcm_file_s));
  if (!priv)
    {
      ferr("ERROR: Failed to allocate file attributes\n");
      return -ENOMEM;
    }

  /* Save the index as the open-specific state in filep->f_priv */

  filep->f_priv = (FAR void *)priv;
  return OK;
}

/****************************************************************************
 * Name: dtcm_close
 ****************************************************************************/

static int dtcm_close(FAR struct file *filep)
{
  FAR struct dtcm_file_s *priv;

  /* Recover our private data from the struct file instance */

  priv = (FAR struct dtcm_file_s *)filep->f_priv;
  DEBUGASSERT(priv);

  /* Release the file attributes structure */

  kmm_free(priv);
  filep->f_priv = NULL;
  return OK;
}

/****************************************************************************
 * Name: dtcm_read
 ****************************************************************************/

static ssize_t dtcm_read(FAR struct file *filep, FAR char *buffer,
                         size_t buflen)
{
  FAR struct dtcm_file_s *priv;
  size_t linesize;
  size_t copysize;
  size_t remaining;
  size_t totalsize;
  struct mallinfo mem;
  off_t offset = filep->f_pos;

  finfo("buffer=%p buflen=%d\n", buffer, (int)buflen);

  /* Recover our private data from the struct file instance */

  priv = (FAR struct dtcm_file_s *)filep->f_priv;
  DEBUGASSERT(priv);


  mm_mallinfo(&g_dtcm_heap, &mem);

  remaining = buflen;
  totalsize = 0;

  linesize = snprintf(priv->line,
                      DTCM_LINELEN,
                      "             total       used       free    largest\n");
  copysize = procfs_memcpy(priv->line, linesize, buffer, remaining, &offset);
  totalsize += copysize;
  buffer    += copysize;
  remaining -= copysize;

  if (totalsize >= buflen)
    {
      return totalsize;
    }

  linesize = snprintf(priv->line,
                      DTCM_LINELEN,
                      "Mem:   %11d%11d%11d%11d\n",
                      mem.arena,
                      mem.uordblks,
                      mem.fordblks,
                      mem.mxordblk);
  copysize = procfs_memcpy(priv->line, linesize, buffer, remaining, &offset);
  totalsize += copysize;

  /* Update the file offset */

  if (totalsize > 0)
    {
      filep->f_pos += totalsize;
    }

  return totalsize;
}

/****************************************************************************
 * Name: dtcm_dup
 *
 * Description:
 *   Duplicate open file data in the new file structure.
 *
 ****************************************************************************/

static int dtcm_dup(FAR const struct file *oldp, FAR struct file *newp)
{
  FAR struct dtcm_file_s *oldpriv;
  FAR struct dtcm_file_s *newpriv;

  finfo("Dup %p->%p\n", oldp, newp);

  /* Recover our private data from the old struct file instance */

  oldpriv = (FAR struct dtcm_file_s *)oldp->f_priv;
  DEBUGASSERT(oldpriv);

  /* Allocate a new container to hold the task and attribute selection */

  newpriv = (FAR struct dtcm_file_s *)kmm_zalloc(sizeof(struct dtcm_file_s));
  if (!newpriv)
    {
      ferr("ERROR: Failed to allocate file attributes\n");
      return -ENOMEM;
    }

  /* The copy the file attributes from the old attributes to the new */

  memcpy(newpriv, oldpriv, sizeof(struct dtcm_file_s));

  /* Save the new attributes in the new file structure */

  newp->f_priv = (FAR void *)newpriv;
  return OK;
}

static int dtcm_stat(const char *relpath, struct stat *buf)
{
  if (strcmp(relpath, "dtcm") != 0)
    {
      ferr("ERROR: relpath is '%s'\n", relpath);
      return -ENOENT;
    }

  buf->st_mode    = S_IFREG | S_IROTH | S_IRGRP | S_IRUSR;
  buf->st_size    = 0;
  buf->st_blksize = 0;
  buf->st_blocks  = 0;

  return OK;
}

#endif /* !CONFIG_DISABLE_MOUNTPOINT && CONFIG_FS_PROCFS */
