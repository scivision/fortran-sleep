cmake_minimum_required(VERSION 3.14)

project(FortranSleep
LANGUAGES C CXX Fortran
)

enable_testing()

include(cmake/options.cmake)
include(cmake/compilers.cmake)

# --- RECOMMENDED: sleep() using C++ stdlib

add_library(sleep_mod OBJECT src/sleep.f90)
set_target_properties(sleep_mod PROPERTIES Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)

add_library(sleep_cpp src/sleep.cpp $<TARGET_OBJECTS:sleep_mod>)
target_compile_features(sleep_cpp PRIVATE cxx_std_11)
target_include_directories(sleep_cpp PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
$<INSTALL_INTERFACE:include>
)

add_executable(main_cpp app/bind.f90)
set_target_properties(main_cpp PROPERTIES LINKER_LANGUAGE Fortran)  # Intel oneAPI Linux needs this
target_link_libraries(main_cpp PRIVATE sleep_cpp)
add_test(NAME CPPsleep COMMAND main_cpp 150)

# --- sleep() using time.h C -- more code but also robust/standard

add_library(sleep_c src/sleep.c $<TARGET_OBJECTS:sleep_mod>)
target_include_directories(sleep_c PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
$<INSTALL_INTERFACE:include>
)

add_executable(main_c app/bind.f90)
target_link_libraries(main_c PRIVATE sleep_c)
add_test(NAME Csleep COMMAND main_c 150)


# --- Fortran bind(C) without specific C code - no error handling, not as simple/robust

add_library(sleep_bind
$<IF:$<BOOL:${MSVC}>,src/reference/windows.f90,src/reference/posix.f90>
)
set_target_properties(sleep_bind PROPERTIES Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/reference/include)
target_include_directories(sleep_bind PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/reference/include>
$<INSTALL_INTERFACE:include>
)

add_executable(main_ref app/bind.f90)
target_link_libraries(main_ref PRIVATE sleep_bind)
add_test(NAME SleepReference COMMAND main_ref 150)

set_tests_properties(SleepReference Csleep CPPsleep PROPERTIES TIMEOUT 3)
