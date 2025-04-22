#pragma once

#include <functional>
#include <vector>

struct NetworkIndex {
    int group, index;
    NetworkIndex(int g, int i) : group(g), index(i) { };
    bool operator==(const NetworkIndex& other) const {
        return group == other.group && index == other.index;
    };
};

typedef std::function<double(const double)> ActivationFunction;
typedef double (*FitnessFunction)(const NetworkIndex, std::vector<double>);
typedef std::vector<double> (*InputFunction)(const NetworkIndex);
typedef void (*OutputFunction)(const NetworkIndex, std::vector<double>);