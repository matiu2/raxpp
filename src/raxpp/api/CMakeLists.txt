project (api)

add_library(api Rackspace.cpp LoadBalancer.cpp Monitoring.cpp HTTPCodeHandler.cpp)
target_link_libraries(api base curlpp11 ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})

add_executable(TestMonitoringAPI TestMonitoring.cpp)
target_link_libraries(TestMonitoringAPI api)
add_test(TestMonitoringAPI TestMonitoring)
