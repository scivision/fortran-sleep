cmake_minimum_required(VERSION 3.15...3.29)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "use out-of-source build
  cmake -Bbuild")
endif()

project(FortranSleep
LANGUAGES C CXX Fortran
)

enable_testing()

set(CMAKE_CXX_STANDARD 11)

include(cmake/compilers.cmake)

include(CheckPIESupported)
check_pie_supported()
set(CMAKE_POSITION_INDEPENDENT_CODE true)

# --- RECOMMENDED: sleep_ms() using C++ stdlib

add_library(sleep_std OBJECT src/sleep_std.f90)
set_property(TARGET sleep_std PROPERTY Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)

add_library(sleep_cpp src/sleep.cpp $<TARGET_OBJECTS:sleep_std>)
target_include_directories(sleep_cpp PUBLIC
"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include;${CMAKE_CURRENT_SOURCE_DIR}/src>"
$<INSTALL_INTERFACE:include>
)

add_executable(main_cpp app/main.f90)
set_property(TARGET main_cpp PROPERTY LINKER_LANGUAGE Fortran)  # Intel oneAPI Linux needs this
target_link_libraries(main_cpp PRIVATE sleep_cpp)
add_test(NAME CPPsleep COMMAND main_cpp 150)

# --- sleep_ms() using time.h C -- more code but also robust/standard

add_library(sleep_c src/sleep.c $<TARGET_OBJECTS:sleep_std>)
target_include_directories(sleep_c PUBLIC
"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include;${CMAKE_CURRENT_SOURCE_DIR}/src>"
$<INSTALL_INTERFACE:include>
)

add_executable(main_c app/main.f90)
target_link_libraries(main_c PRIVATE sleep_c)
add_test(NAME Csleep COMMAND main_c 150)


# --- Fortran bind(C) without specific C code - no error handling, not as simple/robust

add_executable(main_ref app/main.f90
$<IF:$<BOOL:${MSVC}>,reference/windows.f90,reference/posix.f90>)

add_test(NAME SleepReference COMMAND main_ref 150)

set_property(TEST SleepReference Csleep CPPsleep PROPERTY TIMEOUT 3)

file(GENERATE OUTPUT .gitignore CONTENT "*")

install(FILES src/csleep.h TYPE INCLUDE)
