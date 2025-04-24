#include ".hpp"

template <numeral::is T> bool Range<T>::valid_bounds(T min, T max, T step, bool incMin, bool incMax) {
    if (incMin) min += step;
    if (incMax) max -= step;

    return min <= max;
};

template <numeral::is T> constexpr auto Range<T>::smallest_value() const -> T {
    if constexpr (std::is_integral_v<T>)
        return 1;
    else if constexpr (std::is_floating_point_v<T>)
        return std::numeric_limits<T>::epsilon();
    else
        throw std::invalid_argument("Unsupported type for Range.");
};

template <numeral::is T> auto Range<T>::stop() const -> T {
    const T min = get_min();
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
template <numeral::is T> auto Range<T>::rstop() const -> T {
    const T max = get_max();
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

template <numeral::is T> auto Range<T>::raw_min() const -> T { return _min; };
template <numeral::is T> auto Range<T>::raw_max() const -> T { return _max; };

template <numeral::is T> auto Range<T>::min() const -> T { return incMin ? _min : _min + _step; };
template <numeral::is T> auto Range<T>::max() const -> T { return incMax ? _max : _max - _step; };
template <numeral::is T> auto Range<T>::step() const -> T { return _step; };

template <numeral::is T> bool Range<T>::include_left() const { return incMin; };
template <numeral::is T> bool Range<T>::include_right() const { return incMax; };

template <numeral::is T> auto Range<T>::difference() const -> T { return min() - max(); };
template <numeral::is T> int Range<T>::size() const { std::abs(difference() / _step) + 1; };
template <numeral::is T> bool Range<T>::empty() const { return min() == max(); };

template <numeral::is T> bool Range<T>::inside(const T n) const { return n >= min() && n <= max(); };
template <numeral::is T> bool Range<T>::outside(const T n) const { return n < min() || n > max(); };

template <numeral::is T> auto Range<T>::begin() const -> iterator { return iterator(min(), _step); };
template <numeral::is T> auto Range<T>::end() const -> iterator { return iterator(stop() + _step, _step); };
template <numeral::is T> auto Range<T>::rbegin() const -> iterator { return iterator(max(), -_step); };
template <numeral::is T> auto Range<T>::rend() const -> iterator { return iterator(rstop() - _step, -_step); };