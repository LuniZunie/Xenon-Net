#include ".hpp"

template <numeral::is T>
Range<T> Range<T>::operator+(const T val) const {
    return Range<T>(_min + val, _max + val, incMin, incMax, _step);
};
template <numeral::is T>
Range<T> Range<T>::operator+=(const T val) {
    _min += val, _max += val;
    return *this;
};
template <numeral::is T>
Range<T>& Range<T>::operator++() {
    _min += _step, _max += _step;
    return *this;
};
template <numeral::is T>
Range<T> Range<T>::operator++(int) {
    auto temp = *this;
    ++(*this);
    return temp;
};

template <numeral::is T>
Range<T> Range<T>::operator-(const T val) const {
    return Range<T>(_min - val, _max - val, incMin, incMax, _step);
};
template <numeral::is T>
Range<T> Range<T>::operator-=(const T val) {
    _min -= val, _max -= val;
    return *this;
};
template <numeral::is T>
Range<T>& Range<T>::operator--() {
    _min -= _step, _max -= _step;
    return *this;
};
template <numeral::is T>
Range<T> Range<T>::operator--(int) {
    auto temp = *this;
    --(*this);
    return temp;
};