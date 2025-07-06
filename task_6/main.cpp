#include <cassert>
#include <iostream>

#include "format.hpp"

struct Point { int x, y; };
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

void start_tests() {
    assert(format("{0} {1} {2}", "a", "b", "c") == "a b c");
    assert(format("{2} {1} {0}", 1, 2, 3) == "3 2 1");

    assert(format("{0} + {0} = {1}", 2, 4) == "2 + 2 = 4");

    assert(format("{{0}} = {0}", 42) == "{42} = 42");

    assert(format("").empty());

    assert(format("hello") == "hello");

    assert(format("{}") == "{}");
    assert(format("{0}") == "{0}");

    assert(format("{5}", 10, 20) == "{5}");
    assert(format("{-1}", 42) == "{-1}");

    assert(format("{{}}") == "{{}}");
    assert(format("{{{0}}}", 42) == "{{42}}");

    assert(format("{0", 1) == "{0");
    assert(format("0}", 1) == "0}");

    assert(format("{0} {1}", true, false) == "true false");

    int x = 42;
    assert(format("{0}", &x).find("0x") != string::npos);

    assert(format("{0}", Point{1, 2}) == "(1, 2)");
}

int main() {
    start_tests();
    std::cout << "All tests passed" << std::endl;
}