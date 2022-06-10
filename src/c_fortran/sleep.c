#include <stdio.h>

extern void c_sleep(int*);

#ifdef _MSC_VER

#include <windows.h>

// https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleep
void c_sleep(int* milliseconds){
  Sleep(*milliseconds);
}

#else

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
// https://linux.die.net/man/3/usleep
void c_sleep(int* milliseconds)
{

  if (*milliseconds <= 0) {
    fprintf(stderr, "sleep: milliseconds must be strictly positive\n");
    exit(EINVAL);
  }

  int ierr = usleep(*milliseconds * 1000);

  if (ierr != 0){
    if (errno == EINTR){
      fprintf(stderr, "usleep interrupted\n");
    }
    else if(errno == EINVAL){
      fprintf(stderr, "usleep bad milliseconds value\n");
      exit(EINVAL);
    }
    else{
      fprintf(stderr, "usleep error\n");
      exit(1);
    }
  }
}

#endif
