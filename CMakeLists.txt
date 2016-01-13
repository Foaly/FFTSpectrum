cmake_minimum_required(VERSION 3.3)
project(FFTSpectrum)


# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)


# enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Define sources and executable
set(EXECUTABLE_NAME "FFTSpectrum")
set(SOURCE_FILES src/main.cpp
                 src/Application.cpp
                 src/FFT.cpp
                 src/Spectrogram.cpp
                 src/SettingsParser.cpp
                 src/Interpolation.cpp)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})


# Detect and add SFML
set(SFML_ROOT "C:/Libraries/SFML-2.3.2-vc12")
set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()


# Detect and add FFTW
set(FFTW_ROOT "C:/Libraries/fftw-3.3.4-dll32")
set(CMAKE_MODULE_PATH "F:/FFTSpectrum/cmake" ${CMAKE_MODULE_PATH})
# Find FFTW 3
find_package(FFTW COMPONENTS fftw3f-3 REQUIRED)
if(FFTW_FOUND)
    include_directories(${FFTW_INCLUDES})
    target_link_libraries(${EXECUTABLE_NAME} ${FFTW_LIBRARIES})
endif()


# Detect and add Boost
set(BOOST_INCLUDE "C:/Libraries/boost_1_60_0")
include_directories(${BOOST_INCLUDE})
