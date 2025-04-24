#include ".hpp"

template <typename T>
void Registry<T>::clear() {
    for (auto& pair : groups) {
        Group& group = pair.second;
        group.items.clear();
    }
    groups.clear();
};