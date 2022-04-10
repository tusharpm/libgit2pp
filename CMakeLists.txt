#############################################################################
# This file is part of the libgit2pp library
# Copyright (C) 2011 Laszlo Papp
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

# CMake build script for the libgit2pp project
#
# Building (out of source build):
# > mkdir build && cd build
# > cmake .. [-DSETTINGS=VALUE]
# > cmake --build .
#
# Testing:
# > ctest -V
#
# Install:
# > cmake --build . --target install

cmake_minimum_required(VERSION 3.1)
project(libgit2pp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(WIN32) # Dll's into bin
    set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif()

file(STRINGS "include/git2pp.h" GIT2PP_HEADER REGEX "^#define LIBGIT2PP_VERSION \"[^\"]*\"$")
string(REGEX REPLACE "^.*LIBGIT2PP_VERSION \"([0-9]+).*$" "\\1" LIBGIT2PP_VERSION_MAJOR "${GIT2PP_HEADER}")
string(REGEX REPLACE "^.*LIBGIT2PP_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" LIBGIT2PP_VERSION_MINOR  "${GIT2PP_HEADER}")
string(REGEX REPLACE "^.*LIBGIT2PP_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" LIBGIT2PP_VERSION_REV "${GIT2PP_HEADER}")
set(LIBGIT2PP_VERSION_STRING "${LIBGIT2PP_VERSION_MAJOR}.${LIBGIT2PP_VERSION_MINOR}.${LIBGIT2PP_VERSION_REV}")

file(STRINGS "include/git2pp.h" GIT2PP_HEADER_SOVERSION REGEX "^#define LIBGIT2PP_SOVERSION [0-9]+$")
string(REGEX REPLACE "^.*LIBGIT2PP_SOVERSION ([0-9]+)$" "\\1" LIBGIT2PP_SOVERSION "${GIT2PP_HEADER_SOVERSION}")

# Installation paths
set(INSTALL_BIN bin CACHE PATH "Where to install binaries to.")
set(INSTALL_LIB lib CACHE PATH "Where to install libraries to.")
set(INSTALL_INC include CACHE PATH "Where to install headers to.")

# Build options
option(BUILD_TESTS "Build Tests" ON)

# Build Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

set(LIBGIT2_SRC_DIR "" CACHE PATH "Build libgit2 source from this directory.")
option(LIBGIT2_TESTS "Build libgit2 tests" OFF)

if(NOT LIBGIT2_SRC_DIR STREQUAL "")
    message(STATUS "Using libgit2 source from ${LIBGIT2_SRC_DIR}")
    if(NOT LIBGIT2_TESTS)
        set(BUILD_CLAR OFF CACHE STRING "libgit2 tests" FORCE)
    endif()
    if(MSVC)
        set(STDCALL    OFF CACHE STRING "no stdcall" FORCE)
        set(STATIC_CRT OFF CACHE STRING "shared runtime" FORCE)
        # QSharedPointer on opaque libgit structures
        set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   /wd4150")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /wd4150")
    endif()
    add_subdirectory(${LIBGIT2_SRC_DIR} libgit2)
    set(LIBGIT2_INCLUDE_DIR ${LIBGIT2_SRC_DIR}/include)
    set(LIBGIT2_LIBRARIES git2)
    set(LIBGIT2_FOUND 1)
else()
    find_package(Libgit2 REQUIRED)
endif()

if(NOT LIBGIT2_FOUND)
    message(FATAL_ERROR "No libgit2 found!
Install the libgit2 development files to your system.
Alternatively define LIBGIT2_SRC_DIR to point to a libgit2 source directory.")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${LIBGIT2_INCLUDE_DIR})

# Collect files
file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE GIT2PP_HEADERS include/*.h)
file(GLOB_RECURSE GIT2PP_PRIVATE_HEADERS src/private/*.h)

message(STATUS)
message(STATUS "========== LIBGIT2PP Build Information ==========")
message(STATUS "Build Version: ${LIBGIT2PP_VERSION_STRING}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS)
message(STATUS "To change any of these options, override them using -D{OPTION_NAME} on the commandline.")
message(STATUS "To build and install LIBGIT2PP, run \"make\" and \"make install\"")
message(STATUS)

# Compile and link libgit2pp
add_library(git2pp ${SRC} ${GIT2PP_HEADERS} ${GIT2PP_PRIVATE_HEADERS})
target_compile_definitions(git2pp PRIVATE MAKE_LIBGIT2PP_LIB)
target_include_directories(git2pp PRIVATE include)
target_link_libraries(git2pp ${LIBGIT2_LIBRARIES})
set_target_properties(git2pp PROPERTIES
    VERSION ${LIBGIT2PP_VERSION_STRING}
    SOVERSION ${LIBGIT2PP_SOVERSION}
)

# Install
install(TARGETS git2pp
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_LIB}
    ARCHIVE DESTINATION ${INSTALL_LIB}
    INCLUDES DESTINATION ${INSTALL_INC}
)

install(FILES ${GIT2PP_HEADERS} DESTINATION ${INSTALL_INC} COMPONENT Devel)

#Packaging options
set(CPACK_GENERATOR "DEB;RPM;TGZ")
set(CPACK_PACKAGE_NAME "LIBGIT2PP")
set(CPACK_PACKAGE_FILE_NAME "LIBGIT2PP-${LIBGIT2PP_VERSION_STRING}")
set(CPACK_PACKAGE_VERSION "${LIBGIT2PP_VERSION_MAJOR}.${LIBGIT2PP_VERSION_MINOR}.${LIBGIT2PP_VERSION_REV}")
set(CPACK_PACKAGE_CONTACT "Tushar Maheshwari <tushar27192@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libgit2 bindings for C++")
include(InstallRequiredSystemLibraries)
include(CPack)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)
