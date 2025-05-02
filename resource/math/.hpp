#pragma once

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "../concepts/.hpp"
#include "../range/.hpp"

class Math {
    public:
        template <typename T>
        static T log(T x, T base = 10) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::log: x must be a number.");

            if (x <= 0) throw std::out_of_range("Math::log: x must be greater than 0.");
            else if (base <= 1) throw std::out_of_range("Math::log: base must be greater than 1.");

            return std::log(x) / std::log(base);
        };


        template <typename C>
        static typename C::value_type sum(const C& container) {
            static_assert(concepts::container::assert<C>(), "Math::sum: container must be a container.");
            static_assert(concepts::number::assert<typename C::value_type>(concepts::number::Any), "Math::sum: container value type must be a number.");

            typename C::value_type sum = 0;
            for (const auto& value : container)
                sum += value;
            return sum;
        };

        template <typename C>
        static typename C::value_type product(const C& container) {
            static_assert(concepts::container::assert<C>(), "Math::product: container must be a container.");
            static_assert(concepts::number::assert<typename C::value_type>(concepts::number::Any), "Math::product: container value type must be a number.");

            typename C::value_type product = 1;
            for (const auto& value : container)
                product *= value;
            return product;
        };


        template <typename C>
        static typename C::value_type mean(const C& container) {
            static_assert(concepts::container::assert<C>(), "Math::mean: container must be a container.");
            static_assert(concepts::number::assert<typename C::value_type>(concepts::number::Any), "Math::mean: container value type must be a number.");

            return sum(container) / static_cast<typename C::value_type>(container.size());
        };

        template <typename C>
        static typename C::value_type median(const C container) {
            static_assert(concepts::container::assert<C>(), "Math::median: container must be a container.");
            static_assert(concepts::number::assert<typename C::value_type>(concepts::number::Any), "Math::median: container value type must be a number.");

            std::sort(container.begin(), container.end());
            const size_t size = container.size();
            if (size % 2 == 0)
                return (container[size / 2 - 1] + container[size / 2]) / 2;
            else
                return container[size / 2];
        };

        template <typename C>
        static typename C::value_type mode(const C& container) {
            static_assert(concepts::container::assert<C>(), "Math::mode: container must be a container.");

            std::unordered_map<typename C::value_type, int> frequency;
            for (const auto& value : container)
                frequency[value]++;

            return std::max_element(frequency.begin(), frequency.end(),
                [](const auto& a, const auto& b) { return a.second < b.second; }
            )->first;
        };


        template <typename T>
        static T floor(T x) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::floor: x must be a number.");

            return std::floor(x);
        };
        template <typename T>
        static T floor(T x, T p) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::floor: x and p must be a number.");

            return std::floor(x / p) * p;
        };

        template <typename T>
        static T ceil(T x) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::ceil: x must be a number.");

            return std::ceil(x);
        };
        template <typename T>
        static T ceil(T x, T p) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::ceil: x and p must be a number.");

            return std::ceil(x / p) * p;
        };

        template <typename T>
        static T round(T x) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::round: x must be a number.");

            return std::round(x);
        };
        template <typename T>
        static T round(T x, T p) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::round: x and p must be a number.");

            return std::round(x / p) * p;
        };


        template <typename T>
        static T min(const T x, const T y) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::min: x and y must be a number.");

            return std::min(x, y);
        };
        template <typename T>
        static T min(const T x, const Range<T>& r) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::min: x must be a number.");

            return std::min(x, r.max());
        };

        template <typename T>
        static T max(const T x, const T y) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::max: x and y must be a number.");

            return std::max(x, y);
        };
        template <typename T>
        static T max(const T x, const Range<T>& r) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::max: x must be a number.");

            return std::max(x, r.min());
        };

        template <typename T>
        static T clamp(const T x, const T mn, const T mx) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::clamp: x, mn and mx must be a number.");

            return std::clamp(x, mn, mx);
        };
        template <typename T>
        static T clamp(const T x, const Range<T>& r) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::clamp: x must be a number.");

            return std::clamp(x, r.min(), r.max());
        };


        template <typename C>
        static std::vector<typename C::value_type> norm(const C& container) {
            static_assert(concepts::container::assert<C>(), "Math::norm: container must be a container.");
            static_assert(concepts::number::assert<typename C::value_type>(concepts::number::Any), "Math::norm: container value type must be a number.");

            auto min = container[0], max = container[0];
            for (const auto& value : container) {
                if (value < min) min = value;
                if (value > max) max = value;
            }

            std::vector<typename C::value_type> normalized(container.size());
            for (size_t i = 0; i < container.size(); ++i)
                normalized[i] = (container[i] - min) / (max - min);
            return normalized;
        };
        template <typename T>
        static T norm(const T x, const T mn, const T mx) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::norm: x, mn and mx must be a number.");

            return (x - mn) / (mx - mn);
        };
        template <typename T>
        static T norm(const T x, const Range<T>& r) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::norm: x must be a number.");

            return (x - r.min()) / (r.max() - r.min());
        };


        template <typename C>
        static std::vector<typename C::value_type> denorm(const C& container, typename C::value_type mn, typename C::value_type mx) {
            static_assert(concepts::container::assert<C>(), "Math::denorm: container must be a container.");
            static_assert(concepts::number::assert<typename C::value_type>(concepts::number::Any), "Math::denorm: container value type must be a number.");

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
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::denorm: x, mn and mx must be a number.");

            return x * (mx - mn) + mn;
        };
        template <typename T>
        static T denorm(const T x, const Range<T>& r) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::denorm: x must be a number.");

            return x * (r.max() - r.min()) + r.min();
        };

        template <typename C>
        static std::vector<typename C::value_type> map(const C& container, const typename C::value_type mn, const typename C::value_type mx) {
            static_assert(concepts::container::assert<C>(), "Math::map: container must be a container.");
            static_assert(concepts::number::assert<typename C::value_type>(concepts::number::Any), "Math::map: container value type must be a number.");

            std::vector<typename C::value_type> mapped(container.size());
            for (size_t i = 0; i < container.size(); ++i)
                mapped[i] = container[i] * (mx - mn) + mn;
            return mapped;
        };
        template <typename C>
        static std::vector<typename C::value_type> map(const C& container, const Range<typename C::value_type>& r) {
            static_assert(concepts::container::assert<C>(), "Math::map: container must be a container.");

            std::vector<typename C::value_type> mapped(container.size());
            for (size_t i = 0; i < container.size(); ++i)
                mapped[i] = container[i] * (r.max() - r.min()) + r.min();
            return mapped;
        };
        template <typename T>
        static T map(const T x, const T mn1, const T mx1, const T mn2, const T mx2) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::map: x, mn1, mx1, mn2 and mx2 must be a number.");

            return (x - mn1) / (mx1 - mn1) * (mx2 - mn2) + mn2;
        };
        template <typename T>
        static T map(const T x, const Range<T>& r1, const T mn2, const T mx2) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::map: x, mn2 and mx2 must be a number.");

            return (x - r1.min()) / (r1.max() - r1.min()) * (mx2 - mn2) + mn2;
        };
        template <typename T>
        static T map(const T x, const T mn1, const T mx1, const Range<T>& r2) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::map: x, mn1 and mx1 must be a number.");

            return (x - mn1) / (mx1 - mn1) * (r2.max() - r2.min()) + r2.min();
        };
        template <typename T>
        static T map(const T x, const Range<T>& r1, const Range<T>& r2) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::map: x must be a number.");

            return (x - r1.min()) / (r1.max() - r1.min()) * (r2.max() - r2.min()) + r2.min();
        };


        template <typename T>
        static T erf(T x) {
            static_assert(concepts::number::assert<T>(concepts::number::Any), "Math::erf: x must be a number.");

            return std::erf(x);
        };
};