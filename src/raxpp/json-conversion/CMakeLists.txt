project(json-conversion)

add_library(json-conversion LoadBalancer.cpp Monitoring.cpp)
target_link_libraries(json-conversion base)
