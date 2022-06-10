cmake_minimum_required(VERSION 3.14)

project(FortranSleep LANGUAGES C Fortran)

enable_testing()

include(cmake/options.cmake)

# --- Fortran bind(C) without specific C code - no error handling

add_library(sleep_bind OBJECT
$<IF:$<BOOL:${WIN32}>,src/fortran/windows.f90,src/fortran/unix.f90>
)

add_executable(demo_bind app/bind.f90)
target_link_libraries(demo_bind sleep_bind)
add_test(NAME sleep
COMMAND demo_bind 150
)
