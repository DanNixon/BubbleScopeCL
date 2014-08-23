cmake_minimum_required(VERSION 2.8)

project(bubblescope)
set(CMAKE_BUILD_TYPE Debug)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS program_options serialization)

find_package(OpenCV REQUIRED)
find_path(V4L_INCLUDE_DIR NAMES libv4l2.h)
find_library(V4L_LIB NAMES v4l2)

add_executable(bubblescope
  main.cpp
  BubbleScopeUnwrapper.cpp
  CommandLineParser.cpp
  Configuration.cpp
  OCVCapture.cpp
  Timer.cpp
  FrameSource/V4L2Source.cpp
  FrameSource/ImageFileSource.cpp
  FrameSource/VideoFileSource.cpp
  FrameSource/TimelapseSource.cpp
)

target_link_libraries(bubblescope
  ${OpenCV_LIBS}
  ${V4L_LIB}
  ${Boost_LIBRARIES}
)

install(TARGETS bubblescope DESTINATION bin)
