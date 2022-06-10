cmake_minimum_required(VERSION 3.14)

project(FortranSleep LANGUAGES C Fortran)

enable_testing()

include(cmake/options.cmake)

# --- Fortran bind(C) without specific C code - no error handling

add_library(sleep_basic
$<IF:$<BOOL:${MSVC}>,src/fortran/windows.f90,src/fortran/posix.f90>
)
set_target_properties(sleep_basic PROPERTIES Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/b/include)
target_include_directories(sleep_basic PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/b/include>
$<INSTALL_INTERFACE:include>
)

add_executable(basic app/bind.f90)
target_link_libraries(basic PRIVATE sleep_basic)
add_test(NAME SleepBasic
COMMAND basic 150
)

# --- sleep() using C error handling

add_library(sleep_adv src/c_fortran/sleep.c src/c_fortran/sleep.f90)
set_target_properties(sleep_adv PROPERTIES Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/a/include)
target_include_directories(sleep_adv PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/a/include>
$<INSTALL_INTERFACE:include>
)

add_executable(adv app/bind.f90)
target_link_libraries(adv PRIVATE sleep_adv)
add_test(NAME SleepAdv
COMMAND adv 150
)

set_tests_properties(SleepBasic SleepAdv PROPERTIES TIMEOUT 3)
