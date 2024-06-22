add_executable(test_detection test_detection.cpp)

target_link_libraries(test_detection PRIVATE detection)
target_link_libraries(test_detection PRIVATE doctest::doctest)

add_test(NAME test_detection COMMAND test_detection)

add_custom_command(TARGET test_detection POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/data" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data"
        )
