#include ".hpp"

template <container Container>
auto Math::sum(const Container& container) -> typename Container::value_type {
    using T = typename Container::value_type;
    static_assert(std::is_arithmetic_v<T>,, "Math::sum: Container value type must be a numeral type.");
    if (container.empty())
        throw std::invalid_argument("Math::sum: empty container.");

    T total = 0;
    for (const auto& val : container)
        total += val;
    return total;
};