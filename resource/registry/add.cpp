#include ".hpp"

template <typename T>
int Registry<T>::add(T label) {
    if (groups.find(label) == groups.end())
        groups[label] = Group();

    Group& group = groups[label];
    const int id = group.next++;
    group.items.insert(id);

    return id;
};