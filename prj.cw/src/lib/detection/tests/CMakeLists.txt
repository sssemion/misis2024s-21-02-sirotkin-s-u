add_executable(test_detection test_detection.cpp)

target_link_libraries(test_detection PRIVATE detection)
target_link_libraries(test_detection PRIVATE doctest::doctest)

add_test(NAME test_detection COMMAND test_detection)
