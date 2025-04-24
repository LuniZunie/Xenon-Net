#include ".hpp"

template <typename T>
bool Registry<T>::has(const T label) const {
    return groups.find(label) != groups.end();
};

template <typename T>
bool Registry<T>::has(const T label, const int id) const {
    const auto it = groups.find(label);
    if (it == groups.end())
        return false;
    return it->second.items.find(id) != it->second.items.end();
};