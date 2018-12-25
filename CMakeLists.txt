CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(Serial)

file(GLOB SOURCE "src/*.c")
file(GLOB HEADER "header/*.h")

include_directories(header)
add_executable(Serial ${SOURCE})


link_directories(/usr/local/lib)
