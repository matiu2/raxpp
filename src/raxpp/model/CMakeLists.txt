project(model)

add_library(model Monitoring.cpp)
target_link_libraries(model base)
