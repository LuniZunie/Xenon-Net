#pragma once

#include <functional>
#include <vector>

typedef std::function<double(const double)> ActivationFunction;
typedef double (*FitnessFunction)(int, int, std::vector<double>);
typedef void (*OutputFunction)(int, int, std::vector<double>);