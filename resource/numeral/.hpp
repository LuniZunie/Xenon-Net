#pragma once

#include <concepts>

template <typename T>
concept Numeral = std::integral<T> || std::floating_point<T>;