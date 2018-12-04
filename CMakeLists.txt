cmake_minimum_required(VERSION 2.8)
project(feedback)

if (WIN32) # true if windows (32 and 64 bit)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else ()
    SET( CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0" ) 
endif ()

include_directories(kiss_fft130)
include_directories(src)

add_subdirectory(kiss_fft130)
add_subdirectory(src)

add_executable(RemoveFeedback main.cpp)
target_link_libraries(RemoveFeedback fftLibrary sourceFiles)

#add_custom_command(
#        TARGET RemoveFeedback POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_SOURCE_DIR}/wavfiles/example2.wav
#                ${CMAKE_CURRENT_BINARY_DIR}/example2.wav)
