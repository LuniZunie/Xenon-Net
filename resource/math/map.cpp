#include ".hpp"

template <container Container>
auto Math::map(const Container& container, const typename Container::value_type mn, const typename Container::value_type mx) -> std::vector<typename Container::value_type> {
    using T = typename Container::value_type;
    static_assert(std::is_arithmetic_v<T>,, "Math::map: Container value type must be a numeral type.");
    if (container.empty())
        throw std::invalid_argument("Math::map: empty container.");

    return denorm(norm(container), mn, mx);
};

template <container Container>
auto Math::map(const Container& container, const Range<typename Container::value_type>& r) -> std::vector<typename Container::value_type> {
    return map(container, r.min(), r.max());
};

template <numeral::is T>
auto Math::map(const T x, const T mn1, const T mx1, const T mn2, const T mx2) -> T {
    return denorm(norm(x, mn1, mx1), mn2, mx2);
};

template <numeral::is T>
auto Math::map(const T x, const Range<T>& r1, const T mn2, const T mx2) -> T {
    return map(x, r1.min(), r1.max(), mn2, mx2);
};

template <numeral::is T>
auto Math::map(const T x, const T mn1, const T mx1, const Range<T>& r2) -> T {
    return map(x, mn1, mx1, r2.min(), r2.max());
};

template <numeral::is T>
auto Math::map(const T x, const Range<T>& r1, const Range<T>& r2) -> T {
    return map(x, r1.min(), r1.max(), r2.min(), r2.max());
};