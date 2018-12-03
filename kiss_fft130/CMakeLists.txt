cmake_minimum_required(VERSION 2.8)
project(feedback)

## Check for Windows ##
if (WIN32) # true if windows (32 and 64 bit)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else ()
    SET ( CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0" ) 
endif ()

add_library(fftLibrary kiss_fft.h kiss_fft.c tools/kiss_fftr.h tools/kiss_fftr.c)

set_target_properties(fftLibrary PROPERTIES LINKER_LANGUAGE CXX)
