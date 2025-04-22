#pragma once

#include <limits>
#include <stdexcept>

#include "../numeral/.hpp"

template <Numeral T>
class Range {
    private:
        static bool ValidBounds(T min, T max, bool incMin, bool incMax, T step) {
            if (incMin) min += step;
            if (incMax) max -= step;

            return min <= max;
        };

        T min, max, step;
        bool incMin, incMax;

        T SmallestValue() const {
            if constexpr (std::is_integral_v<T>)
                return 1;
            else if constexpr (std::is_floating_point_v<T>)
                return std::numeric_limits<T>::epsilon();
            else
                throw std::invalid_argument("Unsupported type for Range.");
        };

    public:
        Range(T min, T max, bool incMin = true, bool incMax = true, T step = 0) :
            min(min), max(max), step(step),
            incMin(incMin), incMax(incMax) {
            if (step == 0)
                step = SmallestValue();
            else if (step < 0)
                throw std::invalid_argument("Step must be positive.");

            this->step = step;
            if (!Range<T>::ValidBounds(min, max, incMin, incMax, step))
                throw std::invalid_argument("Invalid range bounds.");
        };
        Range(const Range<T>& other) : min(other.min), max(other.max), incMin(other.incMin), incMax(other.incMax), step(other.step) { };
        Range(Range<T>&& other) :
            min(other.min), max(other.max),
            incMin(other.incMin), incMax(other.incMax),
            step(other.step) {
            other.min = 0, other.max = 0, other.step = 0;
        };

        T get_rawMin() const { return this->min; };
        T get_rawMax() const { return this->max; };

        T get_min() const { return incMin ? min : min + step; };
        T get_max() const { return incMax ? max : max - step; };
        T get_step() const { return this->step; };

        bool get_includeLeft() const { return incMin; };
        bool get_includeRight() const { return incMax; };

        T get_diff() const { return get_min() - get_max(); };
        T get_size() const { return (get_min() - get_max()) / step + 1; };

        bool inside(T n) const {
            const T min = get_min(), max = get_max();
            return n >= min && n <= max;
        };
        bool outside(T n) const {
            const T min = get_min(), max = get_max();
            return n < min || n > max;
        };

        T get_end() const {
            const T min = get_min();
            T end;
            if constexpr (std::is_integral_v<T>)
                end = max - (max - min) % step;
            else if constexpr (std::is_floating_point_v<T>)
                end = max - std::fmod(max - min, step);
            else
                throw std::invalid_argument("Unsupported type for Range.");

            if (!incMax && end == max)
                end -= step;
            return end;
        };
        T get_rend() const {
            const T max = get_max();
            T rend;
            if constexpr (std::is_integral_v<T>)
                rend = min + (max - min) % step;
            else if constexpr (std::is_floating_point_v<T>)
                rend = min + std::fmod(max - min, step);
            else
                throw std::invalid_argument("Unsupported type for Range.");

            if (!incMin && rend == min)
                rend += step;
            return rend;
        };

        Range<T> operator+(T value) const { return Range<T>(min + value, max + value, step); };
        Range<T> operator+=(T value) {
            min += value, max += value;
            return *this;
        };
        Range<T> operator++(int) {
            min += step, max += step;
            return *this;
        };

        Range<T> operator-(T value) const { return Range<T>(min - value, max - value, step); };
        Range<T> operator-=(T value) {
            min -= value, max -= value;
            return *this;
        };
        Range<T> operator--(int) {
            min -= step, max -= step;
            return *this;
        };

        Range<T> operator*(T value) const { return Range<T>(min * value, max * value, step); };
        Range<T> operator*=(T value) {
            min *= value, max *= value;
            return *this;
        };

        Range<T> operator/(T value) const {
            if (value == 0)
                throw std::invalid_argument("Division by zero.");

            return Range<T>(min / value, max / value, step);
        };
        Range<T> operator/=(T value) {
            if (value == 0)
                throw std::invalid_argument("Division by zero.");

            min /= value, max /= value;
            return *this;
        };

        Range<T> operator%(T value) const { return Range<T>(min % value, max % value, step); };
        Range<T> operator%=(T value) {
            min %= value, max %= value;
            return *this;
        };

        class Iterator {
            private:
                T current, step;

            public:
                Iterator(T current, T step) : current(current), step(step) { };

                T operator*() const { return current; };
                Iterator& operator++() {
                    current += step;
                    return *this;
                };
                Iterator operator++(int) {
                    Iterator temp = *this;
                    ++(*this);
                    return temp;
                };

                bool operator!=(const Iterator& other) const { return current != other.current; };
                bool operator==(const Iterator& other) const { return current == other.current; };
        };

        Iterator begin() const { return Iterator(get_min(), step); };
        Iterator end() const { return Iterator(get_end() + step, step); };
        Iterator rbegin() const { return Iterator(get_max(), -step); };
        Iterator rend() const { return Iterator(get_rend() - step, -step); };
};