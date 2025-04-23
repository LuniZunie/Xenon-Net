#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>
#include <vector>

class Math {
    public:
        template <typename T>
        static T log(T x, T base = 10) {
            if (x <= 0 || base <= 1)
                throw std::invalid_argument("Math::log: invalid arguments.");
            return std::log(x) / std::log(base);
        };

        template <typename Container>
        static auto sum(const Container& container) -> typename Container::value_type {
            using T = typename Container::value_type;
            T total = 0;
            for (const auto& val : container)
                total += val;
            return total;
        };
        template <typename Container>
        static auto product(const Container& container) -> typename Container::value_type {
            using T = typename Container::value_type;
            T total = 1;
            for (const auto& val : container)
                total *= val;
            return total;
        };

        template <typename Container>
        static auto mean(const Container& container) -> typename Container::value_type {
            using T = typename Container::value_type;
            if (container.empty())
                throw std::invalid_argument("Math::mean: empty container.");

            return sum(container) / static_cast<T>(container.size());
        };
        template <typename Container>
        static auto median(Container container) -> typename Container::value_type {
            using T = typename Container::value_type;
            if (container.empty())
                throw std::invalid_argument("Math::median: empty container.");

            std::sort(container.begin(), container.end());
            size_t size = container.size();
            if (size % 2 == 0)
                return (container[size / 2 - 1] + container[size / 2]) / static_cast<T>(2);
            else
                return container[size / 2];
        };
        template <typename Container>
        static auto mode(const Container& container) -> typename Container::value_type {
            using T = typename Container::value_type;
            if (container.empty())
                throw std::invalid_argument("Math::mode: empty container.");

            std::unordered_map<T, int> frequency;
            for (const auto& val : container)
                frequency[val]++;

            return std::max_element(frequency.begin(), frequency.end(),
                [](const auto& a, const auto& b) {
                    return a.second < b.second;
                }
            )->first;
        };

        template <typename T>
        static auto floor_to(T x, T precision) -> T {
            return std::floor(x / precision) * precision;
        };
        template <typename T>
        static auto ceil_to(T x, T precision) -> T {
            return std::ceil(x / precision) * precision;
        };
        template <typename T>
        static auto round_to(T x, T precision) -> T {
            return std::round(x / precision) * precision;
        };

        template <typename Container>
        static auto norm(const Container& container) -> std::vector<typename Container::value_type> {
            using T = typename Container::value_type;
            if (container.empty())
                throw std::invalid_argument("Math::normalize: empty container.");

            T min = container[0];
            T max = container[0];
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
        template <typename Container>
        static auto denorm(const Container& a, Container::value_type mn, Container::value_type mx) -> std::vector<typename Container::value_type> {
            using T = typename Container::value_type;
            if (a.empty())
                throw std::invalid_argument("Math::denormalize: empty container.");

            std::vector<T> result(a.size());
            std::transform(a.begin(), a.end(), result.begin(),
                [mn, mx](T v) { return mn + (mx - mn) * v; }
            );

            return result;
        };
        template <typename Container>
        static auto lerp(const Container& a, const Container& b, double t) -> std::vector<typename Container::value_type> {
            using T = typename Container::value_type;
            if (a.size() != b.size())
                throw std::invalid_argument("Math::lerp: containers must be of the same size.");

            std::vector<T> result(a.size());
            std::transform(a.begin(), a.end(), b.begin(), result.begin(),
                [t](T a, T b) { return a + t * (b - a); }
            );

            return result;
        };

        template <typename T>
        static T norm_one(const T x, const T mn, const T mx) {
            if (mn == mx)
                return 1;
            return (x - mn) / (mx - mn);
        };
        template <typename T>
        static T denorm_one(const T x, const T mn, const T mx) {
            return mn + (mx - mn) * x;
        };

        static double erf(double x) {
            const double sign = x >= 0 ? 1.0 : -1.0;
            x = std::abs(x);

            const double t = 1.0 / (1.0 + 0.3275911 * x);
            return sign * (1.0 -
                (
                    (
                        (
                            (
                                1.061405429
                            * t - 1.453152027)
                        * t + 1.421413741)
                    * t - 0.284496736)
                * t + 0.254829592) * t * std::exp(-x * x));
        }
};