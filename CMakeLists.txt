cmake_minimum_required(VERSION 2.8)
project(feedback)

if (WIN32) # true if windows (32 and 64 bit)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else ()
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0" ) 
endif ()

find_package(Boost 1.63.0 COMPONENTS iostreams system filesystem)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  include_directories(kiss_fft130)
  include_directories(src)

  add_subdirectory(kiss_fft130)
  add_subdirectory(src)

  add_executable(RemoveFeedback main.cpp)
  target_include_directories(RemoveFeedback PUBLIC lib)
  target_link_libraries(RemoveFeedback fftLibrary sourceFiles ${Boost_LIBRARIES})
endif()
#add_custom_command(
#        TARGET RemoveFeedback POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#                ${CMAKE_SOURCE_DIR}/wavfiles/example2.wav
#                ${CMAKE_CURRENT_BINARY_DIR}/example2.wav)
