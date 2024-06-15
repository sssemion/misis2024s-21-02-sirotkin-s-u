add_executable(cw_dev dev.cpp)
target_link_libraries(cw_dev PRIVATE detection)

add_subdirectory(lib)
include_directories(lib)
