#include <gtest/gtest.h>
#include <sstream>
#include "../serializer.hpp"

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

TEST(SerializerTest, BasicSerialization) {
    Data original{42, true, 100};
    std::stringstream stream;
    Serializer serializer(stream);
    Error err = serializer.save(original);
    ASSERT_EQ(err, Error::NoError);
    std::string serialized = stream.str();
    ASSERT_EQ(serialized, "42 true 100");
    Data restored{0, false, 0};
    Deserializer deserializer(stream);
    err = deserializer.load(restored);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(original, restored);
}

TEST(SerializerTest, BoolValues) {
    // true
    Data data_true{1, true, 2};
    std::stringstream stream_true;
    Serializer ser_true(stream_true);
    ASSERT_EQ(ser_true.save(data_true), Error::NoError);
    ASSERT_EQ(stream_true.str(), "1 true 2");
    Data restored_true{0, false, 0};
    Deserializer des_true(stream_true);
    ASSERT_EQ(des_true.load(restored_true), Error::NoError);
    ASSERT_TRUE(restored_true.b);
    // false
    Data data_false{3, false, 4};
    std::stringstream stream_false;
    Serializer ser_false(stream_false);
    ASSERT_EQ(ser_false.save(data_false), Error::NoError);
    ASSERT_EQ(stream_false.str(), "3 false 4");
    Data restored_false{0, true, 0};
    Deserializer des_false(stream_false);
    ASSERT_EQ(des_false.load(restored_false), Error::NoError);
    ASSERT_FALSE(restored_false.b);
}

TEST(SerializerTest, LargeNumbers) {
    Data data{18446744073709551615ULL, true, 0};
    std::stringstream stream;
    Serializer serializer(stream);
    ASSERT_EQ(serializer.save(data), Error::NoError);
    Data restored{0, false, 0};
    Deserializer deserializer(stream);
    ASSERT_EQ(deserializer.load(restored), Error::NoError);
    ASSERT_EQ(restored.a, 18446744073709551615ULL);
    ASSERT_TRUE(restored.b);
    ASSERT_EQ(restored.c, 0);
}

TEST(SerializerTest, CorruptedData) {
    // bad bool
    std::stringstream stream_bad_bool;
    stream_bad_bool << "42 invalid_bool 100";
    Data data{0, false, 0};
    Deserializer deserializer_bad_bool(stream_bad_bool);
    Error err = deserializer_bad_bool.load(data);
    ASSERT_EQ(err, Error::CorruptedArchive);
    // bad number
    std::stringstream stream_bad_num;
    stream_bad_num << "not_a_number true 100";
    Data data2{0, false, 0};
    Deserializer deserializer_bad_num(stream_bad_num);
    err = deserializer_bad_num.load(data2);
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST(SerializerTest, ZeroAndEdgeValues) {
    // zero
    Data data_zero{0, false, 0};
    std::stringstream stream_zero;
    Serializer serializer_zero(stream_zero);
    ASSERT_EQ(serializer_zero.save(data_zero), Error::NoError);
    ASSERT_EQ(stream_zero.str(), "0 false 0");
    Data restored_zero{123, true, 456};
    Deserializer deserializer_zero(stream_zero);
    ASSERT_EQ(deserializer_zero.load(restored_zero), Error::NoError);
    ASSERT_EQ(restored_zero, data_zero);
    // max
    Data data_max{18446744073709551615ULL, true, 18446744073709551615ULL};
    std::stringstream stream_max;
    Serializer serializer_max(stream_max);
    ASSERT_EQ(serializer_max.save(data_max), Error::NoError);
    Data restored_max{0, false, 0};
    Deserializer deserializer_max(stream_max);
    ASSERT_EQ(deserializer_max.load(restored_max), Error::NoError);
    ASSERT_EQ(restored_max, data_max);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 