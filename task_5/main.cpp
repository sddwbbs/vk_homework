#include <iostream>
#include <cassert>

#include "serializer.hpp"

#include <iostream>
#include <sstream>
#include <cassert>
#include <string>

#include "serializer.hpp"

struct Data {
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }

    bool operator==(const Data& other) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

int main() {
    std::cout << "Программа без тестов. Запустите serializer_tests для тестирования.\n";
    return 0;
}