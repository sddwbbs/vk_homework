#include <cassert>

#include "tests.hpp"
#include "format.hpp"

struct Point { int x, y; };
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

struct A {
    double a;
};

void test_valid_formatting() {
    assert(format("{0} {1} {2}", "a", "b", "c") == "a b c");
    assert(format("{2} {1} {0}", 1, 2, 3) == "3 2 1");
    assert(format("{0} + {0} = {1}", 2, 4) == "2 + 2 = 4");
    assert(format("").empty());
    assert(format("hello") == "hello");
    assert(format("{0} {1}", true, false) == "true false");
    assert(format("{0}", Point{1, 2}) == "(1, 2)");
}

void test_invalid_braces_exceptions() {
    try {
        format("{}", 42);
        assert(false);
    } catch (const InvalidBracesException&) {}

    try {
        format("{0", 1);
        assert(false);
    } catch (const InvalidBracesException&) {}

    try {
        format("0}", 1);
        assert(false);
    } catch (const InvalidBracesException&) {}

    try {
        format("hello } world", 1);
        assert(false);
    } catch (const InvalidBracesException&) {}
}

void test_argument_index_exceptions() {
    try {
        format("{5}", 10, 20);
        assert(false);
    } catch (const ArgumentIndexException&) {}

    try {
        format("{-1}", 42);
        assert(false);
    } catch (const ArgumentIndexException&) {}

    try {
        format("{abc}", 42);
        assert(false);
    } catch (const ArgumentIndexException&) {}

    try {
        format("{999999999999999999999999999999}", 42);
        assert(false);
    } catch (const ArgumentIndexException&) {}
}

void test_edge_cases() {
    assert(format("123") == "123");
    assert(format("{0} {1} {2}", 42, 3.14, "text") == "42 3.14 text");
    assert(format("{0} {0} {0}", "repeat") == "repeat repeat repeat");
    assert(format("{0}", 1234567890) == "1234567890");
    assert(format("{0}", -42) == "-42");
    assert(format("{0}", 3.14159) == "3.14159");
}

void test_concept() {
    static_assert(Streamable<int>);
    static_assert(Streamable<double>);
    static_assert(Streamable<string>);
    static_assert(Streamable<Point>);
    static_assert(!Streamable<A>);
}