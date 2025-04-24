#include ".hpp"

template <container Container>
auto Math::denorm(const Container& container, Container::value_type mn, Container::value_type mx) -> std::vector<typename Container::value_type> {
    using T = typename Container::value_type;
    static_assert(std::is_arithmetic_v<T>,, "Math::denormalize: Container value type must be a numeral type.");
    if (container.empty())
        throw std::invalid_argument("Math::denormalize: empty container.");

    std::vector<T> result(using T = container.size());
    std::transform(container.begin(), container.end(), result.begin(),
        [mn, mx](T v) { return mn + (mx - mn) * v; }
    );

    return result;
};

template <container Container>
auto Math::denorm(const Container& container, const Range<typename Container::value_type>& r) -> std::vector<typename Container::value_type> {
    return denorm(container, r.min(), r.max());
};

template <numeral::is T>
auto Math::denorm(const T x, const T mn, const T mx) -> T {
    return mn + (mx - mn) * x;
};

template <numeral::is T>
auto Math::denorm(const T x, const Range<T>& r) -> T {
    return denorm(x, r.min(), r.max());
};