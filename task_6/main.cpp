#include <iostream>

#include "format.hpp"
#include "tests.hpp"

int main() {
    try {
        test_valid_formatting();
        test_invalid_braces_exceptions();
        test_argument_index_exceptions();
        test_edge_cases();
        test_concept();

        std::cout << "All tests passed" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Tests failed with exception: " << e.what() << std::endl;
    }
}