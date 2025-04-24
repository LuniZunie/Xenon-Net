#pragma once

#include <cmath>
#include <limits>
#include <stdexcept>

#include "../numeral/.hpp"

template <numeral::is T>
class Range {
    private:
        static bool valid_bounds(T min, T max, T step, bool incMin, bool incMax) {
            if (!incMin) min += step;
            if (!incMax) max -= step;
        
            return min <= max;
        };

        T _min, _max, _step;
        bool incMin, incMax;

        constexpr auto smallest_value() const -> T {
            if constexpr (std::is_integral_v<T>)
                return 1;
            else if constexpr (std::is_floating_point_v<T>)
                return std::numeric_limits<T>::epsilon();
            else
                throw std::invalid_argument("Unsupported type for Range.");
        };

        auto stop() const -> T {
            const T min = this->min();
            T end;
            if constexpr (std::is_integral_v<T>)
                end = _max - (_max - min) % _step;
            else if constexpr (std::is_floating_point_v<T>)
                end = _max - std::fmod(_max - min, _step);
            else
                throw std::invalid_argument("Unsupported type for Range.");
        
            if (!incMax && end == _max)
                end -= _step;
            return end;
        };
        auto rstop() const -> T {
            const T max = this->max();
            T rend;
            if constexpr (std::is_integral_v<T>)
                rend = _min + (max - _min) % _step;
            else if constexpr (std::is_floating_point_v<T>)
                rend = _min + std::fmod(max - _min, _step);
            else
                throw std::invalid_argument("Unsupported type for Range.");
        
            if (!incMin && rend == _min)
                rend -= _step;
            return rend;
        };

    public:
        Range(T min, T max, T step, bool l = true, bool r = true) :
            _min(min), _max(max), _step(step),
            incMin(l), incMax(r) {
            if (_step <= 0)
                throw std::invalid_argument("Invalid step value.");
            if (!valid_bounds(_min, _max, _step, incMin, incMax))
                throw std::invalid_argument("Invalid range bounds.");
        };
        Range(T min, T max, bool incMin = true, bool incMax = true) : Range(min, max, smallest_value(), incMin, incMax) { };
        Range(T n) : Range(-n, n, smallest_value(), true, true) { };
        Range(const Range<T>& other) :
            _min(other._min), _max(other._max),
            incMin(other.incMin), incMax(other.incMax),
            _step(other._step) { };
        Range(Range<T>&& other) :
            _min(other._min), _max(other._max),
            incMin(other.incMin), incMax(other.incMax),
            _step(other._step) {
            other._min = 0, other._max = 0, other._step = 0;
        };

        auto raw_min() const -> T { return _min; };
        auto raw_max() const -> T { return _max; };

        auto min() const -> T { return incMin ? _min : _min + _step; };
        auto max() const -> T { return incMax ? _max : _max - _step; };
        auto step() const -> T { return _step; };

        bool include_left() const { return incMin; };
        bool include_right() const { return incMax; };

        auto difference() const -> T { return min() - max(); };
        int size() const { return std::abs(difference() / _step) + 1; };
        bool empty() const { return min() == max(); };

        bool inside(const T n) const { return n >= min() && n <= max(); };
        bool outside(const T n) const { return n < min() || n > max(); };

        Range<T> operator+(const T val) const {
            return Range<T>(min() + val, max() + val, _step, incMin, incMax);
        };
        Range<T> operator+=(const T val) {
            _min += val;
            _max += val;
            return *this;
        };
        Range<T>& operator++() {
            _min += _step;
            _max += _step;
            return *this;
        };
        Range<T> operator++(int) {
            Range<T> temp = *this;
            (*this)++;
            return temp;
        };

        Range<T> operator-(const T val) const {
            return Range<T>(min() - val, max() - val, _step, incMin, incMax);
        };
        Range<T> operator-=(const T val) {
            _min -= val;
            _max -= val;
            return *this;
        };
        Range<T>& operator--() {
            _min -= _step;
            _max -= _step;
            return *this;
        };
        Range<T> operator--(int) {
            Range<T> temp = *this;
            (*this)--;
            return temp;
        };

        class iterator {
            private:
                T _current, _step;

            public:
                iterator(T current, T step) : _current(current), _step(step) { };

                auto operator*() const -> T { return _current; };

                auto operator++() -> iterator& {
                    _current += _step;
                    return *this;
                };
                auto operator++(int) -> iterator {
                    iterator temp = *this;
                    ++(*this);
                    return temp;
                };

                bool operator!=(const iterator& other) const {
                    return _current != other._current;
                };
                bool operator==(const iterator& other) const {
                    return _current == other._current;
                };
        };

        auto begin() const -> iterator {
            return iterator(min(), _step);
        };
        auto end() const -> iterator {
            return iterator(stop() + _step, _step);
        };
        auto rbegin() const -> iterator {
            return iterator(max(), -_step);
        };
        auto rend() const -> iterator {
            return iterator(rstop() - _step, -_step);
        };
};