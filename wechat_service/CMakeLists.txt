cmake_minimum_required(VERSION 3.2)

project(wechart_service)

set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation directory for headers")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} WECHAT_SERVICE_SRC)
add_library(wechart_service STATIC ${WECHAT_SERVICE_SRC})
target_include_directories(wechart_service PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB WECHAT_SERVICE_H ./include/*.h)

install(TARGETS wechart_service
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}")
install(FILES ${WECHAT_SERVICE_H} DESTINATION "${INSTALL_INC_DIR}")