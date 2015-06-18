project(services)

add_library(services LoadBalancer.cpp)
target_link_libraries(services api model)
