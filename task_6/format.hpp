#pragma once

#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::vector;

class FormatException : public std::runtime_error {
public:
    explicit FormatException(const string& message): std::runtime_error(message) {}
};

class InvalidBracesException final : public FormatException {
public:
    explicit InvalidBracesException(const string& message) : FormatException(message) {}
};

class ArgumentIndexException final : public FormatException {
public:
    explicit ArgumentIndexException(const string& message) : FormatException(message) {}
};

template <typename T>
concept Streamable = requires(std::ostream& os, T& value) {
    os << value;
};

template <Streamable T>
string to_string(const T& param) {
    std::ostringstream oss;
    oss << std::boolalpha << param;
    return oss.str();
}

template <typename... Args>
string format(const string& str, Args... args) {
    constexpr size_t args_count = sizeof...(Args);
    vector<string> str_args;
    str_args.reserve(args_count);

    auto convert_args = [&](const auto& arg) {
        str_args.emplace_back(to_string(arg));
    };

    (convert_args(args), ...);

    string res;
    res.reserve(str.size() * 2);

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '{') {
            const size_t end_scope_pos = str.find('}', i + 1);
            if (end_scope_pos == string::npos) {
                throw InvalidBracesException("Unclosed brace at position " + std::to_string(i));
            }

            string idx_str = str.substr(i + 1, end_scope_pos - i - 1);

            if (idx_str.empty()) {
                throw InvalidBracesException("Empty braces are not allowed");
            }

            try {
                const size_t arg_idx = std::stoul(idx_str);
                if (arg_idx >= str_args.size()) {
                    throw ArgumentIndexException("Argument index " + idx_str + " is out of range");
                }
                res += str_args[arg_idx];
            } catch (std::invalid_argument&) {
                throw ArgumentIndexException("Argument index " + idx_str + " is invalid");
            } catch (std::out_of_range&) {
                throw ArgumentIndexException("Argument index " + idx_str + " is out of range");
            }

            i = end_scope_pos;
        } else if (str[i] == '}') {
            throw InvalidBracesException("Unexpected closing brace at position " + std::to_string(i));
        } else {
            res += str[i];
        }
    }

    return res;
}