# Copyright Jed Brown
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice, this
#   list of conditions and the following disclaimer in the documentation and/or
#   other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# This is a modified version of the file taken from here:
# https://github.com/jedbrown/cmake-modules


# - Find FFTW
# Find the native FFTW includes and library
#
#  FFTW_INCLUDES    - where to find fftw3.h
#  FFTW_LIBRARIES   - List of libraries when using FFTW.
#  FFTW_FOUND       - True if FFTW found.
#

# Usage example
# =============
#
# Detect and add FFTW
# set(FFTW_ROOT "/path/to/directory/where/headers_and_compiled_libraries_are")
# set(CMAKE_MODULE_PATH "/path/to/this/file" ${CMAKE_MODULE_PATH})
#
# find_package(FFTW COMPONENTS fftw3f-3 REQUIRED) # here you can also use fftw3-3 or fftw3l-3
# if(FFTW_FOUND)
#     include_directories(${FFTW_INCLUDES})
#     target_link_libraries(${EXECUTABLE_NAME} ${FFTW_LIBRARIES})
# endif()


if (FFTW_INCLUDES)
  # Already in cache, be silent
  set (FFTW_FIND_QUIETLY TRUE)
endif (FFTW_INCLUDES)

# add the header to the include paths
find_path (FFTW_INCLUDES fftw3.h ${FFTW_ROOT})

# the following for each allows you to specify in the calling script which library you want
# to link using COMPONENTS (for example fftw3-3, fftw3f-3 or fftw3l-3)
foreach(FIND_FFTW_COMPONENT ${FFTW_FIND_COMPONENTS})
  string(TOLOWER ${FIND_FFTW_COMPONENT} FIND_FFTW_COMPONENT_LOWER) # convert to lower case
  set(FIND_FFTW_COMPONENT_NAME lib${FIND_FFTW_COMPONENT_LOWER}) # add the lib prefix

  # link the library
  find_library (FFTW_LIBRARIES
          NAMES ${FIND_FFTW_COMPONENT_NAME}
          PATH_SUFFIXES lib64 lib
          PATHS "/usr/local/lib" ${FFTW_ROOT})

endforeach()

MESSAGE(STATUS "Found FFTW 3 in ${FFTW_INCLUDES}")
MESSAGE(STATUS "FFTW 3 Library: ${FFTW_LIBRARIES}")

# handle the QUIETLY and REQUIRED arguments and set FFTW_FOUND to TRUE if
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (FFTW DEFAULT_MSG FFTW_LIBRARIES FFTW_INCLUDES)

mark_as_advanced (FFTW_LIBRARIES FFTW_INCLUDES)
