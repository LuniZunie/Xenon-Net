#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <stdexcept>
#include <vector>

#include "../container/.hpp"
#include "../numeral/.hpp"
#include "../range/.hpp"

class Math {
    public:
        template <numeral::is T>
        static auto log(T x, T base = 10) -> T;

        template <container Container>
        static auto sum(const Container& container) -> typename Container::value_type;
        template <container Container>
        static auto product(const Container& container) -> typename Container::value_type;

        template <container Container>
        static auto mean(const Container& container) -> typename Container::value_type;
        template <container Container>
        static auto median(const Container container) -> typename Container::value_type;
        template <container Container>
        static auto mode(const Container& container) -> typename Container::value_type&;

        template <numeral::is T>
        static inline auto floor(T x) -> T;
        template <numeral::is T>
        static inline auto floor(T x, T p) -> T;

        template <numeral::is T>
        static inline auto ceil(T x) -> T;
        template <numeral::is T>
        static inline auto ceil(T x, T p) -> T;

        template <numeral::is T>
        static inline auto round(T x) -> T;
        template <numeral::is T>
        static inline auto round(T x, T p) -> T;

        template <container Container>
        static auto norm(const Container& container) -> std::vector<typename Container::value_type>;
        template <numeral::is T>
        static auto norm(const T x, const T mn, const T mx) -> T;
        template <numeral::is T>
        static auto norm(const T x, const Range<T>& r) -> T;

        template <container Container>
        static auto denorm(const Container& container, typename Container::value_type mn, typename Container::value_type mx) -> std::vector<typename Container::value_type>;
        template <container Container>
        static auto denorm(const Container& container, const Range<typename Container::value_type>& r) -> std::vector<typename Container::value_type>;
        template <numeral::is T>
        static auto denorm(const T x, const T mn, const T mx) -> T;
        template <numeral::is T>
        static auto denorm(const T x, const Range<T>& r) -> T;

        template <container Container>
        static auto map(const Container& container, const typename Container::value_type mn, const typename Container::value_type mx) -> std::vector<typename Container::value_type>;
        template <container Container>
        static auto map(const Container& container, const Range<typename Container::value_type>& r) -> std::vector<typename Container::value_type>;
        template <numeral::is T>
        static auto map(const T x, const T mn1, const T mx1, const T mn2, const T mx2) -> T;
        template <numeral::is T>
        static auto map(const T x, const Range<T>& r1, const T mn2, const T mx2) -> T;
        template <numeral::is T>
        static auto map(const T x, const T mn1, const T mx1, const Range<T>& r2) -> T;
        template <numeral::is T>
        static auto map(const T x, const Range<T>& r1, const Range<T>& r2) -> T;

        template <numeral::floating::is T>
        static T erf(T x);
};