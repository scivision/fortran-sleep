module sleep_std

use, intrinsic :: iso_c_binding, only : C_INT

implicit none (type, external)

private
public :: sleep_ms

interface
subroutine c_sleep(millseconds) bind(C, name="c_sleep")
import C_INT
integer(C_INT), intent(in), value :: millseconds
end subroutine
end interface

contains

subroutine sleep_ms(millseconds)
integer, intent(in) :: millseconds
call c_sleep(int(millseconds, C_INT))
end subroutine sleep_ms

end module sleep_std
