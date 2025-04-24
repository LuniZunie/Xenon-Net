#include ".hpp"

template <numeral::is T>
inline auto Math::ceil(T x) -> T { return std::ceil(x); };

template <numeral::is T>
inline auto Math::ceil(T x, T p) -> T { return std::ceil(x / p) * p; };