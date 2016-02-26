project (raxpp)

add_subdirectory(base)
add_subdirectory(model)
add_subdirectory(json-conversion)
add_subdirectory(api)

add_library(raxpp LoadBalancer.cpp LoadBalancers.cpp Monitoring.cpp)
target_link_libraries(raxpp model api json-conversion) 

add_executable(TestMonitoring TestMonitoring.cpp)
target_link_libraries(TestMonitoring raxpp)
add_test(TestMonitoring TestMonitoring)
