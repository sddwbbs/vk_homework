#pragma once

#include <cstdint>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
    std::ostream& out_;

    Error process() {
        return Error::NoError;
    }

    template <typename T, typename... Args>
    Error process(T&& arg, Args&&... args) {
        const Error err = processOne(std::forward<T>(arg));

        if (err != Error::NoError) {
            return err;
        }

        if (sizeof...(args) > 0) {
            out_ << Separator;
        }

        return process(std::forward<Args>(args)...);
    }

    Error processOne(const uint64_t value) {
        out_ << value;
        if (out_.bad() || out_.fail()) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error processOne(const bool value) {
        out_ << (value ? "true" : "false");
        if (out_.bad() || out_.fail()) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

public:
    explicit Serializer(std::ostream& out): out_(out) {}

    template <typename T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <typename... Args>
    Error operator()(Args&&... args) {
        return process(std::forward<Args>(args)...);
    }
};

class Deserializer {
    std::istream& in_;

    Error process() {
        return Error::NoError;
    }

    template <typename T, typename... Args>
    Error process(T&& arg, Args&&... args) {
        const Error err = processOne(std::forward<T>(arg));

        if (err != Error::NoError) {
            return err;
        }

        return process(std::forward<Args>(args)...);
    }

    Error processOne(uint64_t& value) {
        in_ >> value;
        if (in_.bad() || in_.fail()) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error processOne(bool& value) {
        std::string text;
        in_ >> text;

        if (in_.bad() || in_.fail()) {
            return Error::CorruptedArchive;
        }

        if (text == "true") {
            value = true;
        } else if (text == "false") {
            value = false;
        } else {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

public:
    explicit Deserializer(std::istream& in): in_(in) {}

    template <typename T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <typename... Args>
    Error operator()(Args&&... args) {
        return process(std::forward<Args>(args)...);
    }
};