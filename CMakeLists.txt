# Required CMAKE version.
cmake_minimum_required (VERSION 3.9)

# Project version
set (PROJECT_VERSION "1.0")

# Project name.
project(ECLcli VERSION ${PROJECT_VERSION} DESCRIPTION "Erik Configuration Language")

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# IPO support.
include (CheckIPOSupported)

# Set the output directory
set (OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_DIR ${CMAKE_BINARY_DIR}/lib)

# Set the output directories.
# set (OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
# set (LIBRARY_DIR ${CMAKE_BINARY_DIR}/lib)
# set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_DIR}/$<0:>)
# set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_DIR}/$<0:>)
# set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/$<0:>)

# Set the default build mode to release if not set.
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif ()

# Set the C standard to use.
set (CMAKE_C_STANDARD 99)
set (CMAKE_C_STANDARD_REQUIRED ON)

# Enable interprocedural optimizations
check_ipo_supported (RESULT ipo_supported)
if (ipo_supported)
    set (CMAKE_INTERPROCEDURAL_OPTIMIZATION true)
endif ()

# Set the C general flags and flags for debug and release.
if (CMAKE_COMPILER_IS_GNUC)
    set (CMAKE_C_FLAGS "-Wall -Wextra")
    set (CMAKE_C_FLAGS_DEBUG "-g")
    set (CMAKE_C_FLAGS_RELEASE "-Ofast -s -flto")
endif ()

# Set the output directories.
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_DIR}/$<0:>)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_DIR}/$<0:>)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/$<0:>)

# Add the subdirectories where to look.
add_subdirectory (ECL)

# Setup the final executable.
add_executable (main main.c)
target_link_libraries (main ECL)
