# Fortran sleep()

OS/compiler-independent "sleep" Fortran subroutine

We provide two examples.

* [src/fortran](./src/fortran) simple `bind(C)` with no custom C code. Works for many applications
* [src/c_fortran](./src/c_fortran/) uses `bind(C)` with error handling code for advanced users.
