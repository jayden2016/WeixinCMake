cmake_minimum_required(VERSION 3.2)

include(Toolchain-Generic-arm.cmake)

project(wechat_sdk)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib CACHE INTERNAL "")
link_directories(${LIBRARY_OUTPUT_PATH}/lib)

#cJSON
include_directories(${CMAKE_SOURCE_DIR}/cJSON)

#Mbedtls
include_directories(${CMAKE_SOURCE_DIR}/Mbedtls)
include_directories(${CMAKE_SOURCE_DIR}/Mbedtls/include)
set(MBEDTLS_CONFIG_FILE "config-mbedtls.h" CACHE INTERNAL "")

#posix
include_directories(${CMAKE_SOURCE_DIR}/posix_include/include)
include_directories(${CMAKE_SOURCE_DIR}/posix_include/include/private)
include_directories(${CMAKE_SOURCE_DIR}/posix_include/FreeRTOS-Plus-POSIX/include)
include_directories(${CMAKE_SOURCE_DIR}/posix_include/FreeRTOS-Plus-POSIX/include/portable)
include_directories(${CMAKE_SOURCE_DIR}/posix_include/FreeRTOS-Plus-POSIX/include/portable/st)

#wexin_port
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build CACHE INTERNAL "")
add_subdirectory(weixin_port)
include_directories(${CMAKE_SOURCE_DIR}/weixin_port/include)

#wechat_service
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build CACHE INTERNAL "")
add_subdirectory(wechat_service)
