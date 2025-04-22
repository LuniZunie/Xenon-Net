#pragma once

#include <cmath>
#include <list>
#include <random>

#include "../numeral/.hpp"
#include "../range/.hpp"

class Random {
    public:
        template <Numeral T>
        static T gen(const Range<T>& range = Range<T>(0, 1, true, false)) {
            const T min = range.get_min(), max = range.get_max(), step = range.get_step();

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

        template<typename T>
        static T list(const std::list<T>& items, int lBound = 0, int rBound = 0) {
            if (lBound < 0 || rBound < 0)
                throw std::out_of_range("Random::list: lBound and rBound must be non-negative.");

            const int min = lBound, max = items.size() - rBound;
            if (min > max)
                throw std::invalid_argument("Random::list: lBound > rBound");
            else if (min == max)
                return *std::next(items.begin(), min);
            else
                return *std::next(items.begin(), Random::gen<int>(Range<int>(min, max, true, false)));
        };

        template <Numeral T>
        static T log(double base = M_E) {
            if (base < 0 || base >= 1)
                throw std::out_of_range("Random::log: base must be in the range [0, 1).");
            return static_cast<T>(std::log(Random::gen<double>()) / std::log(base));
        };

        template <typename T = bool>
        static T condition(double chance = 0.5, T truthy = T{ 1 }, T falsy = T{ 0 }) {
            if (Random::gen<double>() < chance)
                return truthy;
            return falsy;
        };
};