cmake_minimum_required(VERSION 2.8)

project (raxpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
add_definitions(-fPIC)

option(DEBUG_CURL "Turn on verbose logging for curl" OFF)
if (${DEBUG_CURL})
  add_definitions(-DDEBUG_CURL)
endif()

include(cmake/dependencies.cmake)

include_directories(${JSONPP11_SOURCE_DIR} ${CURLPP11_SOURCE_DIR} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(experiments)
