#pragma once

#include <cmath>
#include <limits>
#include <stdexcept>

#include "../concepts/main.hpp"

template <typename T>
concept RangeType = std::is_arithmetic_v<T>;

template <RangeType T>
class Range {
    private:
        static bool valid_bounds(T mn, T mx, T step, bool l, bool r) {
            if (!l) mn += step;
            if (!r) mx -= step;
        
            return mn <= mx;
        };

        T _mn, _mx, _step;
        bool _l, _r;

        constexpr T smallest() const {
            if constexpr (concepts::number::assert<T, concepts::number::Integer>())
                return 1;
            else if constexpr (concepts::number::assert<T, concepts::number::Floating>())
                return std::numeric_limits<T>::epsilon();
            else
                throw std::invalid_argument("Unsupported type for Range.");
        };

        T stop() const {
            const T mn = min();
            T end;
            if constexpr (concepts::number::assert<T, concepts::number::Integer>())
                end = _mx - (_mx - mn) % _step;
            else if constexpr (concepts::number::assert<T, concepts::number::Floating>())
                end = _mx - std::fmod(_mx - mn, _step);
            else
                throw std::invalid_argument("Unsupported type for Range.");
        
            if (!_r && end == _mx)
                end -= _step;
            return end;
        };
        T rstop() const {
            const T mx = max();
            T rend;
            if constexpr (concepts::number::assert<T, concepts::number::Integer>())
                rend = _mn + (mx - _mn) % _step;
            else if constexpr (concepts::number::assert<T, concepts::number::Floating>())
                rend = _mn + std::fmod(mx - _mn, _step);
            else
                throw std::invalid_argument("Unsupported type for Range.");
        
            if (!_l && rend == _mn)
                rend -= _step;
            return rend;
        };

    public:
        Range(T mn, T mx, T step, bool l = true, bool r = true) :
            _mn(mn), _mx(mx), _step(step),
            _l(l), _r(r) {
            if (step <= 0)
                throw std::invalid_argument("Invalid step value.");
            else if (!valid_bounds(mn, mx, step, l, r))
                throw std::invalid_argument("Invalid range bounds.");
        };
        Range(T mn, T mx, bool l = true, bool r = true) : Range(mn, mx, smallest(), l, r) { };
        Range(T n) : Range(-n, n, smallest(), true, true) { };
        Range(const Range<T>& other) :
            _min(other._mn), _mx(other._mx), _step(other._step),
            _l(other._l), _r(other._r) { };
        Range(Range<T>&& other) :
            _mn(other._mn), _mx(other._mx), _step(other._step),
            _l(other._l), _r(other._r) {
                other._mn = 0; other._mx = 0; other._step = 0;
                other._l = true; other._r = true;
        };

        T raw_min() const { return _mn; };
        T raw_max() const { return _mx; };

        T min() const { return _l ? _mn : _mn + _step; };
        T max() const { return _r ? _mx : _mx - _step; };
        T step() const { return _step; };

        bool include_left() const { return _l; };
        bool include_right() const { return _r; };

        T difference() const { return min() - max(); };
        size_t size() const { return std::abs(difference() / _step) + 1; };
        bool empty() const { return min() == max(); };

        bool inside(const T n) const { return n >= min() && n <= max(); };
        bool outside(const T n) const { return n < min() || n > max(); };

        Range<T> operator+(const T val) const {
            const auto m = val * _step;
            return Range<T>(raw_min() + m, raw_max() + m, _step, _l, _r);
        };
        Range<T> operator+=(const T val) {
            const auto m = val * _step;
            _mn += m, _mx += m;
            return *this;
        };
        Range<T>& operator++() {
            _mn += _step, _mx += _step;
            return *this;
        };
        Range<T> operator++(int) {
            Range<T> temp = *this;
            (*this)++;
            return temp;
        };

        Range<T> operator-(const T val) const {
            const auto m = val * _step;
            return Range<T>(raw_min() - m, raw_max() - m, _step, _l, _r);
        };
        Range<T> operator-=(const T val) {
            const auto m = val * _step;
            _mn -= m, _mx -= m;
            return *this;
        };
        Range<T>& operator--() {
            _mn -= _step, _mx -= _step;
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