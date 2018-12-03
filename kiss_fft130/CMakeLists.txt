cmake_minimum_required(VERSION 2.8)
project(feedback)

## Check for Windows ##
if (WIN32) # true if windows (32 and 64 bit)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else ()
    SET( CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0" ) 
endif ()

include_directories(tools)

add_library(fftLibrary _kiss_fft_guts.h kiss_fft.h kiss_fft.c kissfft.hh tools/kfc.h tools/kiss_fftnd.h tools/kiss_fftndr.h tools/kiss_fftr.h tools/fftutil.c tools/kfc.c tools/kiss_fastfir.c tools/kiss_fftnd.c tools/kiss_fftndr.c tools/kiss_fftr.c tools/psdpng.c)

set_target_properties(fftLibrary PROPERTIES LINKER_LANGUAGE CXX)