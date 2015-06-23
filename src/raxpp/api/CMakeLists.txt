project (api)

add_library(api Rackspace.cpp LoadBalancer.cpp)
target_link_libraries(api base curlpp11 ${LIB_CPP} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})
