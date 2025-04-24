#include ".hpp"

template <container Container>
auto Math::mode(const Container& container) -> typename Container::value_type& {
    using T = typename Container::value_type;
    static_assert(std::is_arithmetic_v<T>,, "Math::mode: Container value type must be a numeral type.");
    if (container.empty())
        throw std::invalid_argument("Math::mode: empty container.");

    std::unordered_map<T, int> fq;
    for (const auto& val : container)
        fq[val]++;

    auto mx = 0;
    auto mode = container[0];
    for (const auto& [val, c] : fq) {
        if (c > mx) {
            mx = c;
            mode = val;
        }
    }

    return mode;
};