#pragma once

#include <concepts>
#include <iterator>

template <typename T>
concept container = requires(T c) {
    typename T::value_type;
    { c.begin() } -> std::input_iterator;
    { c.end() } -> std::input_iterator;
};