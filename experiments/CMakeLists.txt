project(experiments)

add_executable(login login.cpp ${CURLPP11_FILE})
target_link_libraries(login ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})
