
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <ecr/config.h>
#include <stdio.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * hello_main
 ****************************************************************************/

#if defined(BUILD_MODULE)
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
  printf("Hello, World!!\n");
  return 0;
}