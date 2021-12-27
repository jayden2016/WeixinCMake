cmake_minimum_required(VERSION 3.2)

include(Toolchain-Generic-arm.cmake)

project(weixin_test)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib CACHE INTERNAL "")
link_directories(${LIBRARY_OUTPUT_PATH}/lib)

add_subdirectory(weixin_port)
include_directories(${CMAKE_SOURCE_DIR}/weixin_port/include)
