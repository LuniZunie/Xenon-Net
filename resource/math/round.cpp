#include ".hpp"

template <numeral::is T>
inline auto Math::round(T x) -> T {
    return std::round(x);
};

template <numeral::is T>
inline auto Math::round(T x, T p) -> T {
    return std::round(x / p) * p;
};