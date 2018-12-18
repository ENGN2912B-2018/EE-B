cmake_minimum_required(VERSION 2.8)
project(feedback)

## Check for Windows ##
if (WIN32) # true if windows (32 and 64 bit)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else ()
    SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0" ) 
endif ()

add_library(sourceFiles FileReader.h FileReader.cpp FeedbackID.h FeedbackID.cpp)

set_target_properties(sourceFiles PROPERTIES LINKER_LANGUAGE CXX)
