#include "../.hpp"

template <numeral::is T>
auto Range<T>::iterator::operator*() const -> T {
    return _current;
};

template <numeral::is T>
auto Range<T>::iterator::operator++() -> iterator& {
    _current += _step;
    return *this;
};
template <numeral::is T>
auto Range<T>::iterator::operator++(int) -> iterator {
    auto temp = *this;
    ++(*this);
    return temp;
};

template <numeral::is T>
bool Range<T>::iterator::operator!=(const iterator& other) const {
    return _current != other._current;
};
template <numeral::is T>
bool Range<T>::iterator::operator==(const iterator& other) const {
    return _current == other._current;
};