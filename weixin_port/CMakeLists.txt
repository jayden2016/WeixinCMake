cmake_minimum_required(VERSION 3.2)

project(WeixinChannel)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src WEIXIN_CHANNEL_SRC)
add_library(WeixinChannel STATIC ${WEIXIN_CHANNEL_SRC})
target_include_directories(WeixinChannel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)