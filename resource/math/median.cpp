#include ".hpp"

template <container Container>
auto Math::median(const Container container) -> typename Container::value_type {
    using T = typename Container::value_type;
    static_assert(std::is_arithmetic_v<T>,, "Math::median: Container value type must be a numeral type.");
    if (container.empty())
        throw std::invalid_argument("Math::median: empty container.");

    if (container.size() == 1)
        return container[0];
    else if (container.size() == 2)
        return (container[0] + container[1]) / static_cast<T>(2);

    std::sort(container.begin(), container.end());
    const int size = container.size();
    if (size % 2 == 0)
        return (container[size / 2 - 1] + container[size / 2]) / static_cast<T>(2);
    else
        return container[size / 2];
};