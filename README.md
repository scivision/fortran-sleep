# Fortran standard sleep()

[![ci_cmake](https://github.com/scivision/fortran-sleep/actions/workflows/ci_cmake.yml/badge.svg)](https://github.com/scivision/fortran-sleep/actions/workflows/ci_cmake.yml)

OS/compiler-independent "sleep" Fortran subroutine

The simplest robust way to implement Fortran "sleep(milliseconds)" in a standard way is using C++ as in
[sleep.cpp](./src/sleep.cpp)
and
[sleep.f90](./src/sleep.f90)

Two other methods are shown.
POSIX nanosleep in [sleep.c](./src/sleep.c) takes noticeably more code than C++.
Simply binding to C functions is less robust in [reference](./src/reference/).
