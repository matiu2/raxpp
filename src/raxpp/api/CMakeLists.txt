project (api)

add_library(api Rackspace.cpp LoadBalancer.cpp Monitoring.cpp HTTPCodeHandler.cpp)
target_link_libraries(api base curlpp11 ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})

add_executable(TestMonitoring TestMonitoring.cpp)
target_link_libraries(TestMonitoring api)
