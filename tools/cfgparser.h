
#ifndef __TOOLS_CFGPARSER_H
#define __TOOLS_CFGPARSER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <limits.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LINESIZE  ( PATH_MAX > 256 ? PATH_MAX : 256 )

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct variable_s
{
  struct variable_s *flink;
  char *var;
  char *val;
  char storage[1];
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern char line[LINESIZE+1];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void parse_file(FILE *stream, struct variable_s **list);
struct variable_s *find_variable(const char *varname, struct variable_s *list);

#endif /* __TOOLS_CFGPARSER_H */
