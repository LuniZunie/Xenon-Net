#include ".hpp"

template <numeral::is T>
auto Math::log(T x, T base) -> T {
    if (x <= 0 || base <= 1)
        throw std::invalid_argument("Math::log: invalid arguments.");
    return std::log(x) / std::log(base);
};