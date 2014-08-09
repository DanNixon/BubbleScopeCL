cmake_minimum_required(VERSION 2.8)

project(bubblescope)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV REQUIRED)
find_path(V4L_INCLUDE_DIR NAMES libv4l2.h)
find_library(V4L_LIB NAMES v4l2)

add_executable(bubblescope
  main.cpp
  BubbleScopeUnwrapper.cpp
  bubblescope_capture_params.cpp
  command_line_params.cpp
  Timer.cpp
  OCVCapture.cpp
  FrameSource/V4L2Source.cpp
  FrameSource/ImageFileSource.cpp
  FrameSource/VideoFileSource.cpp
  FrameSource/TimelapseSource.cpp
  config_file.cpp
)

target_link_libraries(bubblescope
  ${OpenCV_LIBS}
  ${V4L_LIB}
)

install(TARGETS bubblescope DESTINATION bin)
