project (raxpp)

add_subdirectory(base)
add_subdirectory(model)
add_subdirectory(validation)
add_subdirectory(json-conversion)
add_subdirectory(api)

add_library(raxpp LoadBalancer.cpp LoadBalancers.cpp)
target_link_libraries(raxpp model api json-conversion)
