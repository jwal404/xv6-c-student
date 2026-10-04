#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int mem = getusedmem();
  printf("Used memory: %d bytes\n", mem);
  exit(0);
}
