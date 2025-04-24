#include ".hpp"

template <container Container>
auto Math::product(const Container& container) -> typename Container::value_type {
    using T = typename Container::value_type;
    static_assert(std::is_arithmetic_v<T>, "Math::product: Container value type must be a numeral type.");
    if (container.empty())
        throw std::invalid_argument("Math::product: empty container.");

    T total = 1;
    for (const auto& val : container)
        total *= val;
    return total;
};