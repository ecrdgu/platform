
#ifndef __TOOLS_CSVPARSER_H
#define __TOOLS_CSVPARSER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>
#include <limits.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LINESIZE      (PATH_MAX > 256 ? PATH_MAX : 256)

#define MAX_FIELDS    16
#define MAX_PARMSIZE  128
#define NAME_INDEX    0
#define HEADER_INDEX  1
#define COND_INDEX    2
#define RETTYPE_INDEX 3
#define PARM1_INDEX   4

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern bool g_debug;
extern char g_line[LINESIZE+1];
extern char g_parm[MAX_FIELDS][MAX_PARMSIZE];
extern int  g_lineno;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

char *read_line(FILE *stream);
int parse_csvline(char *ptr);

#endif /* __TOOLS_CSVPARSER_H */
