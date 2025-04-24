#include ".hpp"

template <typename T, numeral::floating::is U>
auto Random::condition(U chance, T t, T f) -> T {
    if (chance < 0 || chance > 1)
        throw std::out_of_range("Random::condition: chance must be in the range [0, 1].");
    return generate<U>() < chance ? t : f;
};