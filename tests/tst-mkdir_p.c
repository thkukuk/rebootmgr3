
#include <stdlib.h>
#include <string.h>
 #include <sys/stat.h>

#include "util.h"

/* test that mkdir_p works */

#define TEST_PATH "tests/a/b/c/d/9/8/7/e"

int
main(void)
{
  int r = mkdir_p(TEST_PATH, 0755);

  if (r < 0)
    {
      fprintf(stderr, "mkdir_p failed: %s\n", strerror(-r));
      return 1;
    }

  struct stat st;
  if (stat(TEST_PATH, &st) < 0)
    {
      fprintf(stderr, "stat failed: %m\n");
      return 1;
    }

  return 0;
}
