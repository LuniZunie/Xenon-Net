#pragma once

#include <optional>
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

        ~Registry() { clr(); };

        int add(const T label) {
            if (groups.find(label) == groups.end())
                groups[label] = Group();

            Group& group = groups[label];
            const int id = group.next++;
            group.items.insert(id);

            return id;
        };
        bool has(const T label, const std::optional<int> id = std::nullopt) {
            if (id.has_value()) {
                if (groups.find(label) == groups.end())
                    return false;

                const Group& group = groups[label];
                return group.items.find(id.value()) != group.items.end();
            } else
                return groups.find(label) != groups.end();
        };
        bool del(const T label, const std::optional<int> id = std::nullopt) {
            if (groups.find(label) == groups.end())
                return false;

            if (id.has_value()) {
                Group& group = groups[label];
                const int value = id.value();

                if (group.items.find(value) == group.items.end())
                    return false;

                group.items.erase(value);
                if (group.items.empty())
                    groups.erase(label);

                return true;
            } else {
                Group& group = groups[label];
                for (const auto& id : group.items)
                    group.items.erase(id);

                groups.erase(label);
                return true;
            }
        };

        void clr() {
            for (auto& pair : groups) {
                Group& group = pair.second;
                group.items.clear();
            }
            groups.clear();
        };

        Registry& operator=(const Registry&) = delete;
        Registry& operator=(Registry&&) = delete;
};