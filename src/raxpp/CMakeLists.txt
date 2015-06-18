project (raxpp)

add_subdirectory(base)
add_subdirectory(model)
add_subdirectory(json-conversion)
add_subdirectory(api)
add_subdirectory(services)

add_library(raxpp empty.cpp)
target_link_libraries(raxpp api json-conversion services)
