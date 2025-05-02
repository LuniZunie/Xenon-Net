#pragma once

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

template <typename T>
class Registry {
    private:
        struct Group {
            size_t next = 0;
            std::unordered_set<int> items;
            Group() : items() { };
        };
        std::unordered_map<T, Group> groups;

    public:
        Registry() : groups() { };
        Registry(const Registry&) = delete;
        Registry(Registry&&) = delete;

        ~Registry() { clear(); };

        size_t add(const T label) {
            if (groups.find(label) == groups.end())
                groups[label] = Group();
        
            Group& group = groups[label];
            const size_t id = ++group.next;
            group.items.insert(id);
        
            return id;
        };

        bool has(const T label) const { return groups.find(label) != groups.end(); };
        bool has(const T label, const size_t id) const {
            auto it = groups.find(label);
            if (it == groups.end())
                return false;
            return it->second.items.find(id) != it->second.items.end();
        };

        bool empty() const { return groups.empty(); };

        bool erase(const T label) {
            auto it = groups.find(label);
            if (it == groups.end())
                return false;
        
            auto& items = it->second.items;
            for (auto it2 = items.begin(); it2 != items.end(); ++it2)
                items.erase(*it2);
        
            groups.erase(label);
            return true;
        };
        bool erase(const T label, const size_t id) {
            auto it = groups.find(label);
            if (it == groups.end())
                return false;
        
            auto& items = it->second.items;
            if (items.find(id) == items.end())
                return false;
        
            items.erase(id);
            if (items.empty())
                groups.erase(label);
        
            return true;
        };

        void clear() {
            for (auto it = groups.begin(); it != groups.end(); ++it) {
                auto& items = it->second.items;
                for (auto it2 = items.begin(); it2 != items.end(); ++it2)
                    items.erase(*it2);
            }
            groups.clear();
        };

        std::unordered_set<int>::const_iterator begin(const T label) const {
            auto it = groups.find(label);
            if (it == groups.end())
                throw std::out_of_range("Label not found in registry.");
            return it->second.items.cbegin();
        };
        std::unordered_set<int>::const_iterator end(const T label) const {
            auto it = groups.find(label);
            if (it == groups.end())
                throw std::out_of_range("Label not found in registry.");
            return it->second.items.cend();
        };

        std::unordered_set<int>::const_iterator rbegin(const T label) const {
            auto it = groups.find(label);
            if (it == groups.end())
                throw std::out_of_range("Label not found in registry.");
            return it->second.items.crbegin();
        };
        std::unordered_set<int>::const_iterator rend(const T label) const {
            auto it = groups.find(label);
            if (it == groups.end())
                throw std::out_of_range("Label not found in registry.");
            return it->second.items.crend();
        };

        Registry& operator=(const Registry&) = delete;
        Registry& operator=(Registry&&) = delete;
};