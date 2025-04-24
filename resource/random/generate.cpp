#include ".hpp"

template <numeral::is T>
auto Random::generate(const Range<T>& range) -> T {
    const T min = range.min(), max = range.max(), step = range.step();

    std::random_device rd;
    std::mt19937 gen(rd());
    if constexpr (std::is_integral_v<T>) { // equally generate integers between min and max while on step
        std::uniform_int_distribution<T> dis(min, max);
        const T random = dis(gen);
        return random - (random - min) % step;
    } else if constexpr (std::is_floating_point_v<T>) { // equally generate floating point numbers between min and max while on step
        std::uniform_real_distribution<T> dis(min, max + step);
        const T random = dis(gen);
        return random - fmod(random - min, step);
    } else
        throw std::invalid_argument("Unsupported type for Random.");
};

template <numeral::is T>
auto Random::generate(T mn, T mx, T step, bool l, bool r) -> T {
    return generate(Range<T>(mn, mx, step, l, r));
};

template <numeral::is T>
auto Random::generate(T mn, T mx, bool l, bool r) -> T {
    return generate(Range<T>(mn, mx, l, r));
};

template <numeral::is T>
auto Random::generate(T n) -> T {
    return generate(Range<T>(n));
};

template <numeral::is T>
auto Random::generate() -> T {
    return generate(Range<T>(0, 1, true, false));
};