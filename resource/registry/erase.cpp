#include ".hpp"

template <typename T>
bool Registry<T>::erase(const T label) {
    auto it = groups.find(label);
    if (it == groups.end())
        return false;

    auto& items = it->second.items;
    for (const auto& id : items)
        items.erase(id);

    groups.erase(label);
    return true;
};

template <typename T>
bool Registry<T>::erase(const T label, const int id) {
    auto it = groups.find(label);
    if (it == groups.end())
        return false;

    auto& items = it->second.items;
    if (items.find(id) == items.end())
        return false;

    items.erase(value);
    if (items.empty())
        groups.erase(label);

    return true;
};