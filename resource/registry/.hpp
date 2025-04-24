#pragma once

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

template <typename T>
class Registry {
    private:
        struct Group {
            int next;
            std::unordered_set<int> items;
            Group() : next(0), items() { };
        };
        std::unordered_map<T, Group> groups;

    public:
        Registry() : groups() { };
        Registry(const Registry&) = delete;
        Registry(Registry&&) = delete;

        ~Registry() { clear(); };

        int add(const T label);

        bool has(const T label) const;
        bool has(const T label, const int id) const;

        bool empty() const;

        bool erase(const T label);
        bool erase(const T label, const int id);

        void clear();

        std::unordered_set<int>::iterator begin(const T label) const;
        std::unordered_set<int>::iterator end(const T label) const;

        std::unordered_set<int>::iterator cbegin(const T label) const;
        std::unordered_set<int>::iterator cend(const T label) const;

        std::unordered_set<int>::iterator rbegin(const T label) const;
        std::unordered_set<int>::iterator rend(const T label) const;

        Registry& operator=(const Registry&) = delete;
        Registry& operator=(Registry&&) = delete;
};