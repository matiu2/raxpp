cmake_minimum_required(VERSION 2.8)

project (curlpp11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(cmake/dependencies.cmake)

include_directories(${JSONPP11_SOURCE_DIR} ${CURLPP11_SOURCE_DIR} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

add_subdirectory(experiments)
