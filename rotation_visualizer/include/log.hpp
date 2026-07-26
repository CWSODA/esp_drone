#pragma once

#include <iostream>

template <typename T>
void log_msg(const char* msg, const T& value) {
    std::cout << msg << value << std::endl;
}

inline void log_msg(const char* msg) { std::cout << msg << std::endl; }