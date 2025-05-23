#pragma once

#include <cmath>
#include <limits>
#include <stdexcept>

#include "../concepts/.hpp"

template <typename T>
concept RangeType = std::is_arithmetic_v<T>;

template <RangeType T>
class Range {
    private:
        static bool valid_bounds(T min, T max, T step, bool incMin, bool incMax) {
            if (!incMin) min += step;
            if (!incMax) max -= step;
        
            return min <= max;
        };

        T _min, _max, _step;
        bool incMin, incMax;

        constexpr T smallest_value() const {
            if constexpr (concepts::number::assert<T>(concepts::number::Integer))
                return 1;
            else if constexpr (concepts::number::assert<T>(concepts::number::Floating))
                return std::numeric_limits<T>::epsilon();
            else
                throw std::invalid_argument("Unsupported type for Range.");
        };

        T stop() const {
            const T min = this->min();
            T end;
            if constexpr (concepts::number::assert<T>(concepts::number::Integer))
                end = _max - (_max - min) % _step;
            else if constexpr (concepts::number::assert<T>(concepts::number::Floating))
                end = _max - std::fmod(_max - min, _step);
            else
                throw std::invalid_argument("Unsupported type for Range.");
        
            if (!incMax && end == _max)
                end -= _step;
            return end;
        };
        T rstop() const {
            const T max = this->max();
            T rend;
            if constexpr (concepts::number::assert<T>(concepts::number::Integer))
                rend = _min + (max - _min) % _step;
            else if constexpr (concepts::number::assert<T>(concepts::number::Floating))
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
            else if (!valid_bounds(_min, _max, _step, incMin, incMax))
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

        T raw_min() const { return _min; };
        T raw_max() const { return _max; };

        T min() const { return incMin ? _min : _min + _step; };
        T max() const { return incMax ? _max : _max - _step; };
        T step() const { return _step; };

        bool include_left() const { return incMin; };
        bool include_right() const { return incMax; };

        T difference() const { return min() - max(); };
        int size() const { return std::abs(difference() / _step) + 1; };
        bool empty() const { return min() == max(); };

        bool inside(const T n) const { return n >= min() && n <= max(); };
        bool outside(const T n) const { return n < min() || n > max(); };

        Range<T> operator+(const T val) const {
            const auto m = val * _step;
            return Range<T>(raw_min() + m, raw_max() + m, _step, incMin, incMax);
        };
        Range<T> operator+=(const T val) {
            const auto m = val * _step;
            _min += m, _max += m;
            return *this;
        };
        Range<T>& operator++() {
            _min += _step, _max += _step;
            return *this;
        };
        Range<T> operator++(int) {
            Range<T> temp = *this;
            (*this)++;
            return temp;
        };

        Range<T> operator-(const T val) const {
            const auto m = val * _step;
            return Range<T>(raw_min() - m, raw_max() - m, _step, incMin, incMax);
        };
        Range<T> operator-=(const T val) {
            const auto m = val * _step;
            _min -= m, _max -= m;
            return *this;
        };
        Range<T>& operator--() {
            _min -= _step, _max -= _step;
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

                T operator*() const { return _current; };

                iterator& operator++() {
                    _current += _step;
                    return *this;
                };
                iterator operator++(int) {
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

        iterator begin() const {
            return iterator(min(), _step);
        };
        iterator end() const {
            return iterator(stop() + _step, _step);
        };
        iterator rbegin() const {
            return iterator(max(), -_step);
        };
        iterator rend() const {
            return iterator(rstop() - _step, -_step);
        };
};