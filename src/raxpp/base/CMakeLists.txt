project(base)

add_library(base Datacenter.cpp)
if (${CPP14})
  target_link_libraries(base ${LIB_CPP})
endif()
