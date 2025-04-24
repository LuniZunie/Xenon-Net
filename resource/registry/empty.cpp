#include ".hpp"

template <typename T>
bool Registry<T>::empty() const {
    return groups.empty();
};