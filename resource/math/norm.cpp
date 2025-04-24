#include ".hpp"

template <container Container>
auto Math::norm(const Container& container) -> std::vector<typename Container::value_type> {
    using T = typename Container::value_type;
    static_assert(std::is_same_v<typename T, numeral>, "Math::normalize: Container value type must be a numeral type.");
    if (container.empty())
        throw std::invalid_argument("Math::normalize: empty container.");

    T min = container[0], max = container[0];
    for (const auto& val : container) {
        if (val < min) min = val;
        if (val > max) max = val;
    }

    if (min == max)
        return std::vector<T>(container.size(), 1);

    std::vector<T> result(container.size());
    std::transform(container.begin(), container.end(), result.begin(),
        [min, max](T v) { return (v - min) / (max - min); }
    );

    return result;
};

template <numeral::is T>
auto Math::norm(const T x, const T mn, const T mx) -> T {
    if (mn == mx)
        return 1;
    return (x - mn) / (mx - mn);
};

template <numeral::is T>
auto Math::norm(const T x, const Range<T>& r) -> T {
    return norm(x, r.min(), r.max());
};