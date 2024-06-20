add_subdirectory(main)
add_subdirectory(lib)
include_directories(lib)

option(TEST "TEST" OFF)

if(TEST)
    include(FetchContent)
    FetchContent_Declare(
            doctest
            GIT_REPOSITORY https://github.com/doctest/doctest.git
            GIT_TAG v2.4.11
    )
    FetchContent_MakeAvailable(doctest)
endif()
