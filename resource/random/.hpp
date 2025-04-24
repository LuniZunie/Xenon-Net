#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <iterator>
#include <random>

#include "../container/.hpp"
#include "../math/.hpp"
#include "../numeral/.hpp"
#include "../range/.hpp"

class Random {
    public:
        template <numeral::is T>
        static auto generate(const Range<T>& range) -> T {
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
        static auto generate(T mn, T mx, T step, bool l = true, bool r = false) -> T {
            return generate(Range<T>(mn, mx, step, l, r));
        };
        template <numeral::is T>
        static auto generate(T mn, T mx, bool l = true, bool r = false) -> T {
            return generate(Range<T>(mn, mx, l, r));
        };
        template <numeral::is T>
        static auto generate(T n) -> T {
            return generate(Range<T>(n));
        };
        template <numeral::is T>
        static auto generate() -> T {
            return generate(Range<T>(0, 1, true, false));
        };

        template <numeral::is T, numeral::floating::is U>
        static auto log(U base = M_E) -> T {
            if (base < 0 || base >= 1)
                throw std::out_of_range("Random::log: base must be in the range [0, 1).");
            return static_cast<T>(std::log(generate<U>()) / std::log(base));
        };

        template <typename T = bool, numeral::floating::is U>
        static auto condition(U chance = 0.5, T t = static_cast<T>(1), T f = static_cast<T>(0)) -> T {
            if (chance < 0 || chance > 1)
                throw std::out_of_range("Random::condition: chance must be in the range [0, 1].");
            return generate<U>() < chance ? t : f;
        };

        template <container Container, container Weights>
        static auto pick(const Container& container, const Weights& weights, unsigned int l = 0U, unsigned int r = 0U) -> typename Container::value_type* {
            using T = typename Container::value_type;
            using W = typename Weights::value_type;
            static_assert(std::is_arithmetic_v<W> && !std::is_signed_v<W>, "Random::pick: Weights must be unsigned integral type.");
        
            if (container.empty())
                return nullptr;
            else if (container.size() != weights.size())
                return nullptr;
        
            r = container.size() - r;
            if (l >= r)
                return nullptr;
        
            W weight = 0;
            auto end = std::next(weights.begin(), r);
            for (auto it = std::next(weights.begin(), l); it != end; ++it)
                weight += *it;
        
            if (weight == 0)
                return nullptr;
        
            W n = generate(0, weight);
            for (auto it = std::next(weights.begin(), l); it != end; ++it) {
                n -= *it;
                if (n < 0)
                    return &std::next(container.begin(), std::distance(weights.begin(), it));
            }
        };
        template <container Container>
        static auto pick(const Container& container, unsigned int l = 0U, unsigned int r = 0U) -> typename Container::value_type* {
            using T = typename Container::value_type;
            if (container.empty())
                return nullptr;
        
            r = container.size() - r;
            if (l >= r)
                return nullptr;
        
            return &std::next(container.begin(), generate(l, r));
        };
};