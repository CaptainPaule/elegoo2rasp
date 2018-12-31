CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(libSerial)

file(GLOB SOURCE "src/*.c")
file(GLOB HEADER "header/*.h")

include_directories(header)
add_executable(libSerial ${SOURCE})

link_directories(/usr/local/lib)
