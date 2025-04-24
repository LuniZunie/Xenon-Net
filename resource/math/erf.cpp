#include ".hpp"

template <numeral::floating::is T>
auto Math::erf(T x) -> T {
    const T sign = x >= 0 ? 1 : -1;
    x = std::abs(x);

    const T t = 1.0 / (1.0 + 0.3275911 * x);
    return sign * (1.0 -
        (
            (
                (
                    (
                        1.061405429
                    * t - 1.453152027)
                * t + 1.421413741)
            * t - 0.284496736)
        * t + 0.254829592) * std::exp(-x * x));
};