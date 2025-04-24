#include ".hpp"

template <numeral::is T>
inline auto Math::floor(T x) -> T {
    return std::floor(x);
};

template <numeral::is T>
inline auto Math::floor(T x, T p) -> T {
    return std::floor(x / p) * p;
};