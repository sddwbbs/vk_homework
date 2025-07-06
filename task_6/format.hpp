#pragma once

#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::vector;

template <typename T>
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
            if (size_t end_scope_pos = str.find('}', i + 1);
                end_scope_pos != string::npos) {
                string idx_str = str.substr(i + 1, end_scope_pos - i - 1);

                try {
                    if (!idx_str.empty()) {
                        size_t arg_idx = std::stoul(idx_str);
                        if (arg_idx >= str_args.size()) {
                            throw std::out_of_range("index out of range");
                        }

                        res += str_args[arg_idx];
                    } else {
                        res += str[i];
                        continue;
                    }
                } catch (...) {
                    res += str[i];
                    continue;
                }

                i = end_scope_pos;
                continue;
                }
        }
        res += str[i];
    }

    return res;
}