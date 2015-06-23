project(base)

add_library(base Datacenter.cpp)
target_link_libraries(base ${LIB_CPP})
