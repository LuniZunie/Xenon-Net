#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "../concepts/main.hpp"
#include "../range/main.hpp"

namespace math {
    template <typename N>
    static constexpr N smallest() {
        if constexpr (concepts::number::assert<N, concepts::number::Integer>())
            return 1;
        else if constexpr (concepts::number::assert<N, concepts::number::Floating>())
            return std::numeric_limits<N>::epsilon();
        else
            throw std::invalid_argument("Unsupported type for Range.");
    };


    template <typename N>
    static inline N round(const N n) {
        static_assert(concepts::number::assert<N>(), "math::round - `n` must be arithmetic");
        
        return std::round(n);
    };
    template <typename N>
    static inline N round(const N n, const N p) {
        static_assert(concepts::number::assert<N>(), "math::round - `n` and `p` must be arithmetic");
        
        return std::round(n * p) / p;
    };

    template <typename N>
    static inline N floor(const N n) {
        static_assert(concepts::number::assert<N>(), "math::floor - `n` must be arithmetic");
        
        return std::floor(n);
    };
    template <typename N>
    static inline N floor(const N n, const N p) {
        static_assert(concepts::number::assert<N>(), "math::floor - `n` and `p` must be arithmetic");
        
        return std::floor(n * p) / p;
    };

    template <typename N>
    static inline N ceil(const N n) {
        static_assert(concepts::number::assert<N>(), "math::ceil - `n` must be arithmetic");
        
        return std::ceil(n);
    };
    template <typename N>
    static inline N ceil(const N n, const N p) {
        static_assert(concepts::number::assert<N>(), "math::ceil - `n` and `p` must be arithmetic");
        
        return std::ceil(n * p) / p;
    };


    template <typename N>
    static N log(N x, N base = 10) {
        static_assert(concepts::number::assert<N>(), "math::log - `x` must be arithmetic");

        if (x <= 0) throw std::out_of_range("math::log - `x` must be greater than 0.");
        else if (base <= 1) throw std::out_of_range("math::log - base must be greater than 1.");

        return std::log(x) / std::log(base);
    };


    template <typename C>
    static typename C::value_type sum(const C& container) {
        static_assert(concepts::container::assert<C>(), "math::sum - `container` must be a container");
        static_assert(concepts::number::assert<typename C::value_type>(), "math::sum - `container` must have an arithmetic value type");
        
        typename C::value_type sum = 0;
        for (const auto& n : container)
            sum += n;
        return sum;
    };

    template <typename C>
    static typename C::value_type product(const C& container) {
        static_assert(concepts::container::assert<C>(), "math::product - `container` must be a container");
        static_assert(concepts::number::assert<typename C::value_type>(), "math::product - `container` must have an arithmetic value type");
        
        typename C::value_type prod = 1;
        for (const auto& n : container)
            prod *= n;
        return prod;
    };


    template <typename C>
    static typename C::value_type mean(const C& container) {
        static_assert(concepts::container::assert<C>(), "math::mean - `container` must be a container.");
        static_assert(concepts::number::assert<typename C::value_type>(), "math::mean - `container` must have an arithmetic value type.");

        return sum(container) / static_cast<typename C::value_type>(container.size());
    };

    template <typename C>
    static typename C::value_type median(const C container) {
        static_assert(concepts::container::assert<C>(), "math::median - `container` must be a container.");
        static_assert(concepts::number::assert<typename C::value_type>(), "Math::median - `container` must have an arithmetic value type.");

        std::sort(container.begin(), container.end());
        const size_t size = container.size();
        if (size % 2 == 0)
            return (container[size / 2 - 1] + container[size / 2]) / 2;
        else
            return container[size / 2];
    };

    template <typename C>
    static typename C::value_type mode(const C& container) {
        static_assert(concepts::container::assert<C>(), "math::mode - `container` must be a container.");

        std::unordered_map<typename C::value_type, int> frequency;
        for (const auto& value : container)
            frequency[value]++;

        return std::max_element(frequency.begin(), frequency.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; }
        )->first;
    };


    template <typename N>
    static N clamp_lower(const N x, const N mn) {
        static_assert(concepts::number::assert<N>(), "math::clamp_lower - `x` and `mn` must be arithmetic");

        return std::max(x, mn);
    };
    template <typename N>
    static N clamp_lower(const N x, const Range<N>& r) {
        static_assert(concepts::number::assert<N>(), "math::clamp_lower - `x` must be arithmetic");

        return std::max(x, r.min());
    };

    template <typename N>
    static N clamp_upper(const N x, const N mx) {
        static_assert(concepts::number::assert<N>(), "math::clamp_upper - `x` and `mx` must be arithmetic");

        return std::min(x, mx);
    };
    template <typename N>
    static N clamp_upper(const N x, const Range<N>& r) {
        static_assert(concepts::number::assert<N>(), "math::clamp_upper - `x` must be arithmetic");

        return std::min(x, r.max());
    };

    template <typename N>
    static N clamp(const N x, const N mn, const N mx) {
        static_assert(concepts::number::assert<N>(), "math::clamp - `x`, `mn` and `mx` must be arithmetic");

        return std::clamp(x, mn, mx);
    };
    template <typename N>
    static N clamp(const N x, const Range<N>& r) {
        static_assert(concepts::number::assert<N>(), "math::clamp - `x` must be arithmetic");

        return std::clamp(x, r.min(), r.max());
    };


    template <typename C>
    static std::vector<typename C::value_type> norm(const C& container) {
        static_assert(concepts::container::assert<C>(), "Math::norm: container must be a container.");
        static_assert(concepts::number::assert<typename C::value_type>(), "Math::norm: container value type must be a number.");

        auto mn = container[0], mx = container[0];
        for (const auto& val : container) {
            if (val < mn) mn = val;
            if (val > mx) mx = val;
        }

        std::vector<typename C::value_type> normalized(container.size());
        for (size_t i = 0; i < container.size(); ++i)
            normalized[i] = (container[i] - mn) / (mx - mn);
        return normalized;
    };
    template <typename T>
    static T norm(const T x, const T mn, const T mx) {
        static_assert(concepts::number::assert<T>(), "Math::norm: x, mn and mx must be a number.");

        return (x - mn) / (mx - mn);
    };
    template <typename T>
    static T norm(const T x, const Range<T>& r) {
        static_assert(concepts::number::assert<T>(), "Math::norm: x must be a number.");

        return (x - r.min()) / (r.max() - r.min());
    };

    template <typename C>
    static std::vector<typename C::value_type> denorm(const C& container, typename C::value_type mn, typename C::value_type mx) {
        static_assert(concepts::container::assert<C>(), "Math::denorm: container must be a container.");
        static_assert(concepts::number::assert<typename C::value_type>(), "Math::denorm: container value type must be a number.");

        std::vector<typename C::value_type> denormalized(container.size());
        for (size_t i = 0; i < container.size(); ++i)
            denormalized[i] = container[i] * (mx - mn) + mn;
        return denormalized;
    };
    template <typename C>
    static std::vector<typename C::value_type> denorm(const C& container, const Range<typename C::value_type>& r) {
        static_assert(concepts::container::assert<C>(), "Math::denorm: container must be a container.");

        std::vector<typename C::value_type> denormalized(container.size());
        for (size_t i = 0; i < container.size(); ++i)
            denormalized[i] = container[i] * (r.max() - r.min()) + r.min();
        return denormalized;
    };
    template <typename T>
    static T denorm(const T x, const T mn, const T mx) {
        static_assert(concepts::number::assert<T>(), "Math::denorm: x, mn and mx must be a number.");

        return x * (mx - mn) + mn;
    };
    template <typename T>
    static T denorm(const T x, const Range<T>& r) {
        static_assert(concepts::number::assert<T>(), "Math::denorm: x must be a number.");

        return x * (r.max() - r.min()) + r.min();
    };

    template <typename C>
    static C map(const C& container, const typename C::value_type mn, const typename C::value_type mx) {
        static_assert(concepts::container::assert<C>(), "Math::map: container must be a container.");
        static_assert(concepts::number::assert<typename C::value_type>(), "Math::map: container value type must be a number.");

        std::vector<typename C::value_type> mapped(container.size());
        for (size_t i = 0; i < container.size(); ++i)
            mapped[i] = (container[i] - mn) / (mx - mn);
        return mapped;
    };
    template <typename C>
    static C map(const C& container, const Range<typename C::value_type>& r) {
        static_assert(concepts::container::assert<C>(), "Math::map: container must be a container.");

        std::vector<typename C::value_type> mapped(container.size());
        for (size_t i = 0; i < container.size(); ++i)
            mapped[i] = (container[i] - r.min()) / (r.max() - r.min());
        return mapped;
    };
    template <typename T>
    static T map(const T x, const T mn1, const T mx1, const T mn2, const T mx2) {
        static_assert(concepts::number::assert<T>(), "Math::map: x, mn1, mx1, mn2 and mx2 must be a number.");

        return (x - mn1) / (mx1 - mn1) * (mx2 - mn2) + mn2;
    };
    template <typename T>
    static T map(const T x, const Range<T>& r1, const T mn2, const T mx2) {
        static_assert(concepts::number::assert<T>(), "Math::map: x, mn2 and mx2 must be a number.");

        return (x - r1.min()) / (r1.max() - r1.min()) * (mx2 - mn2) + mn2;
    };
    template <typename T>
    static T map(const T x, const T mn1, const T mx1, const Range<T>& r2) {
        static_assert(concepts::number::assert<T>(), "Math::map: x, mn1, mx1 must be a number.");

        return (x - mn1) / (mx1 - mn1) * (r2.max() - r2.min()) + r2.min();
    };
    template <typename T>
    static T map(const T x, const Range<T>& r1, const Range<T>& r2) {
        static_assert(concepts::number::assert<T>(), "Math::map: x must be a number.");

        return (x - r1.min()) / (r1.max() - r1.min()) * (r2.max() - r2.min()) + r2.min();
    };
};