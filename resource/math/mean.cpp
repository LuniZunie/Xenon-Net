#include ".hpp"

template <container Container>
auto Math::mean(const Container& container) -> typename Container::value_type {
    using T = typename Container::value_type;
    static_assert(std::is_arithmetic_v<T>,, "Math::mean: Container value type must be a numeral type.");
    if (container.empty())
        throw std::invalid_argument("Math::mean: empty container.");

    T total = 0;
    for (const auto& val : container)
        total += val;
    return total / static_cast<T>(container.size());
};