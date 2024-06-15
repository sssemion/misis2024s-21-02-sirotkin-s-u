#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <iostream>

void hello_world() {
    std::cout << "hello world" << std::endl;
}

TEST_CASE("Example test") {
    hello_world();
}