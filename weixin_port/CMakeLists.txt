cmake_minimum_required(VERSION 3.2)

project(WeixinChannel)

set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation directory for headers")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src WEIXIN_CHANNEL_SRC)
add_library(WeixinChannel STATIC ${WEIXIN_CHANNEL_SRC})
target_include_directories(WeixinChannel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB WEIXIN_CHANNEL_H ./include/*.h)

install(TARGETS WeixinChannel
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}")
install(FILES ${WEIXIN_CHANNEL_H} DESTINATION "${INSTALL_INC_DIR}")