#if defined(__linux__) && !defined(_DEFAULT_SOURCE)
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>

#include "csleep.h"


#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleep
void c_sleep(int milliseconds){
  Sleep(milliseconds);
}

#else

#if defined(HAVE_NANOSLEEP)
#include <time.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <errno.h>
// https://linux.die.net/man/3/usleep
void c_sleep(int milliseconds)
{

  if (milliseconds <= 0) {
    fprintf(stderr, "sleep: milliseconds must be strictly positive\n");
    exit(EINVAL);
  }

  int ierr = 0;

#if defined(HAVE_NANOSLEEP)
  struct timespec t;

  t.tv_sec = milliseconds / 1000;
  t.tv_nsec = (milliseconds % 1000) * 1000000;

  ierr = nanosleep(&t, NULL);
#else
  ierr = usleep(milliseconds * 1000);
#endif

  if (ierr != 0){
    switch(errno){
      case EINTR:
        fprintf(stderr, "c_sleep() interrupted\n");
        break;
      case EINVAL:
        fprintf(stderr, "c_sleep() bad milliseconds value\n");
        exit(EINVAL);
      case EFAULT:
        fprintf(stderr, "c_sleep() bad milliseconds value\n");
        exit(EFAULT);
      case ENOSYS:
        fprintf(stderr, "c_sleep() not supported on this system\n");
        exit(ENOSYS);
      default:
        fprintf(stderr, "c_sleep() error\n");
        exit(1);
    }
  }
}

#endif
