#include ".hpp"

template <numeral::is T, numeral::floating::is U>
auto Random::log(U base) -> T {
    if (base < 0 || base >= 1)
        throw std::out_of_range("Random::log: base must be in the range [0, 1).");
    return static_cast<T>(std::log(generate<U>()) / std::log(base));
};