#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <iterator>
#include <random>

#include "../concepts/main.hpp"
#include "../math/main.hpp"
#include "../range/main.hpp"

class Random {
    public:
        template <typename T>
        static T generate(const Range<T>& range) {
            static_assert(concepts::number::assert<T>(), "Random::generate: range must be a number.");

            const T mn = range.min(); const T mx = range.max();
            const T step = range.step();
        
            std::random_device rd;
            std::mt19937 gen(rd());
            if constexpr (std::is_integral_v<T>) { // equally generate integers between min and max while on step
                std::uniform_int_distribution<T> dis(mn, mx);
                const T rng = dis(gen);
                return rng - (rng - mn) % step;
            } else if constexpr (std::is_floating_point_v<T>) { // equally generate floating point numbers between min and max while on step
                std::uniform_real_distribution<T> dis(mn, mx + step);
                const T rng = dis(gen);
                return rng - fmod(rng - mn, step);
            } else
                throw std::invalid_argument("Unsupported type for Random.");
        };
        template <typename T>
        static T generate(T mn, T mx, T step, bool l = true, bool r = false) {
            static_assert(concepts::number::assert<T>(), "Random::generate: range must be a number.");

            return generate(Range<T>(mn, mx, step, l, r));
        };
        template <typename T>
        static T generate(T mn, T mx, bool l = true, bool r = false) {
            static_assert(concepts::number::assert<T>(), "Random::generate: range must be a number.");

            return generate(Range<T>(mn, mx, l, r));
        };
        template <typename T>
        static T generate(T n) {
            static_assert(concepts::number::assert<T>(), "Random::generate: range must be a number.");
            
            return generate(Range<T>(n));
        };
        template <typename T>
        static T generate() {
            static_assert(concepts::number::assert<T>(), "Random::generate: range must be a number.");

            return generate(Range<T>(0, 1, true, false));
        };


        template <typename T, typename U>
        static T log(U base = M_E) {
            static_assert(concepts::number::assert<T>(), "Random::log: base must be a number.");
            static_assert(concepts::number::assert<U, concepts::number::Floating>(), "Random::log: base must be a number.");

            if (base < 0 || base >= 1)
                throw std::out_of_range("Random::log: base must be in the range [0, 1).");

            return math::log(generate<U>(), base);
        };


        template <typename T = bool, typename U>
        static T condition(U chance = 0.5, T t = static_cast<T>(1), T f = static_cast<T>(0)) {
            static_assert(concepts::number::assert<U, concepts::number::Floating>(), "Random::condition: chance must be a number.");
            
            if (chance < 0 || chance > 1)
                throw std::out_of_range("Random::condition: chance must be in the range [0, 1].");

            return generate<U>() < chance ? t : f;
        };

    
        template <typename C, typename W>
        static typename C::value_type pick(const C& container, const W& weights, size_t l = 0, size_t r = 0) {
            static_assert(concepts::container::assert<C>(), "Random::pick: Container must be a container.");
            static_assert(concepts::number::assert<typename C::value_type>(), "Random::pick: Container value type must be a number.");
            static_assert(concepts::container::assert<W>(), "Random::pick: Weights must be a container.");
            static_assert(concepts::number::assert<typename W::value_type>(), "Random::pick: Weights value type must be a number.");
        
            if (container.empty())
                throw std::out_of_range("Random::pick: container is empty.");
            else if (container.size() != weights.size())
                throw std::out_of_range("Random::pick: container and weights must be the same size.");
        
            r = container.size() - r;
            if (l >= r)
                throw std::out_of_range("Random::pick: not enough elements in range.");
        
            W weight = 0;
            auto end = std::next(weights.begin(), r);
            for (auto it = std::next(weights.begin(), l); it != end; ++it) {
                if (*it < 0)
                    throw std::out_of_range("Random::pick: weights must be greater than 0.");
        
                weight += *it;
            }
        
            if (weight == 0)
                throw std::out_of_range("Random::pick: weights must be greater than 0.");
        
            W n = generate(0, weight);
            for (auto it = std::next(weights.begin(), l); it != end; ++it) {
                n -= *it;
                if (n < 0)
                    return std::next(container.begin(), std::distance(weights.begin(), it));
            }
        };
        template <typename C>
        static typename C::value_type pick(const C& container, size_t l = 0, size_t r = 0) {
            static_assert(concepts::container::assert<C>(), "Random::pick: Container must be a container.");
            
            if (container.empty())
                throw std::out_of_range("Random::pick: container is empty.");
        
            r = container.size() - r;
            if (l >= r)
                throw std::out_of_range("Random::pick: not enough elements in range.");
        
            return std::next(container.begin(), generate(l, r));
        };
};