cmake_minimum_required(VERSION 3.28)
project(2GoImgBytes C)

set(CMAKE_C_STANDARD 11)

add_executable(2GoImgBytes main.c)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(2GoImgBytes ${PNG_LIBRARIES})
