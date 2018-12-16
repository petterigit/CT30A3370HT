#include "types.h"
#include "stat.h"
#include "user.h"


/* syscall.c: syscalls[] ? */
int
main(int argc, char *argv[]) // OWNCODE
{
  int readcount;
  int i;
  readcount = getreadcount();

  for(i = 1; i < 2; i++)
    printf(1, "%d%s", readcount, i+1 < 2 ? " " : "\n");
  exit();
}
