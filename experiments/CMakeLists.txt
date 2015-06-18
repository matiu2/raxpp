project(experiments)

add_executable(login login.cpp ${CURLPP11_FILE})
target_link_libraries(login ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src) # So we can include <raxpp/...>
add_executable(lb-block lb-block.cpp)
target_link_libraries(lb-block raxpp)
