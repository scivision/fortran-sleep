module sleep_std
use, intrinsic :: iso_c_binding, only : C_INT, C_LONG
implicit none (type, external)

private
public :: sleep_ms


type, bind(C) :: timespec
integer(C_INT) :: tv_sec
integer(C_LONG) :: tv_nsec
end type timespec


interface
integer(C_INT) function usleep(usec) bind (C)
!! int usleep(useconds_t usec);
!! https://linux.die.net/man/3/usleep
import C_INT
integer(C_INT), value, intent(in) :: usec
end function

integer(C_INT) function nanosleep(request, remainder) bind(C)
!! this works on Linux, but freezes on MacOS and MinGW
import C_INT, timespec
type(timespec), intent(in) :: request
type(timespec), intent(out) :: remainder
end function
end interface

contains

subroutine sleep_ms(millisec)
integer, intent(in) :: millisec
integer(C_INT) :: ierr

! type(timespec) :: request, remainder
ierr = usleep(int(millisec * 1000, C_INT))
if (ierr/=0) error stop 'problem with usleep() system call'

! request%tv_sec = int(millisec / 1000, C_INT)
! request%tv_nsec = int(modulo(millisec, 1000), C_LONG) * 1000000

! ierr = nanosleep(request, remainder)
! if (ierr/=0) error stop 'problem with nanosleep() system call'

end subroutine sleep_ms

end module sleep_std
