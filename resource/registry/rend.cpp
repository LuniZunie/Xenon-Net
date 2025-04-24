#include ".hpp"

template <typename T>
std::unordered_set<int>::iterator Registry<T>::rend(const T label) const {
    const auto it = groups.find(label);
    if (it == groups.end())
        throw std::invalid_argument("Registry: label not found.");
    return it->second.items.begin();
};