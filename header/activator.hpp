#pragma once

#include <cmath>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../typedef/functions.hpp"

class Activator {
    private:
        static std::unordered_map<std::string, std::string> ParseAliases(const std::unordered_map<std::string, std::unordered_set<std::string>>& aliases) {
            std::unordered_map<std::string, std::string> result;
            for (const auto& [ key, values ] : aliases) {
                for (const auto& value : values) {
                    if (result.find(value) != result.end())
                        throw std::runtime_error("Activator: duplicate alias");
                    result[value] = key;
                }
            }
            return result;
        };

        static const std::unordered_map<std::string, std::string> aliases;
        static const std::unordered_map<std::string, int> constants;

        static ActivationFunction f_binary_step() {
            return [](const double x) { return (x>=0) ? 1.0 : 0.0; };
        };
        static ActivationFunction f_exponential_linear_unit(const double a) {
            return [a](const double x) { return (x>=0) ? x : a*(std::exp(x)-1); };
        };
        static ActivationFunction f_gaussian() {
            return [](const double x) { return std::exp(-x*x); };
        };
        static ActivationFunction f_gaussian_error_linear_unit() {
            return [](const double x) { return 0.5*x*(1+std::erf(x/std::sqrt(2))); };
        };
        static ActivationFunction f_identity() {
            return [](const double x) { return x; };
        };
        static ActivationFunction f_leaky_rectified_linear_unit(const double a) {
            return [a](const double x) { return (x>=0) ? x : a*x; };
        };
        static ActivationFunction f_parametric_rectified_linear_unit(const double a) {
            return [a](const double x) { return (x>=0) ? x : a*x; };
        };
        static ActivationFunction f_rectified_linear_unit() {
            return [](const double x) { return std::max(0.0,x); };
        };
        static ActivationFunction f_scaled_exponential_linear_unit(const double a, const double b) {
            return [a,b](const double x) { return (x>=0) ? a*x : b*(std::exp(x)-1); };
        };
        static ActivationFunction f_sigmoid() {
            return [](const double x) { return 1.0/(1.0+std::exp(-x)); };
        };
        static ActivationFunction f_sigmoid_linear_unit() {
            return [](const double x) { return x/(1.0+std::exp(-x)); };
        };
        static ActivationFunction f_smoothed_hyperbolic_tangent(const double a, const double b, const double c, const double d) {
            return [a,b,c,d](const double x) { return (std::exp(a*x)-std::exp(-b*x))/(std::exp(c*x)+std::exp(-d*x)); };
        };
        static ActivationFunction f_softplus() {
            return [](const double x) { return std::log(1.0+std::exp(x)); };
        };
        static ActivationFunction f_hyperbolic_tangent() {
            return [](const double x) { return std::tanh(x); };
        };

        static std::string s_binary_step() {
            return "[](const double x) { return (x>=0) ? 1.0 : 0.0; }";
        };
        static std::string s_exponential_linear_unit(const double a) {
            return "[a](const double x) { return (x>=0) ? x : a*(std::exp(x)-1); }";
        };
        static std::string s_gaussian() {
            return "[](const double x) { return std::exp(-x*x); }";
        };
        static std::string s_gaussian_error_linear_unit() {
            return "[](const double x) { return 0.5*x*(1+std::erf(x/std::sqrt(2))); }";
        };
        static std::string s_identity() {
            return "[](const double x) { return x; }";
        };
        static std::string s_leaky_rectified_linear_unit(const double a) {
            return "[a](const double x) { return (x>=0) ? x : a*x; }";
        };
        static std::string s_parametric_rectified_linear_unit(const double a) {
            return "[a](const double x) { return (x>=0) ? x : a*x; }";
        };
        static std::string s_rectified_linear_unit() {
            return "[](const double x) { return std::max(0.0,x); }";
        };
        static std::string s_scaled_exponential_linear_unit(const double a, const double b) {
            return "[a,b](const double x) { return (x>=0) ? a*x : b*(std::exp(x)-1); }";
        };
        static std::string s_sigmoid() {
            return "[](const double x) { return 1.0/(1.0+std::exp(-x)); }";
        };
        static std::string s_sigmoid_linear_unit() {
            return "[](const double x) { return x/(1.0+std::exp(-x)); }";
        };
        static std::string s_smoothed_hyperbolic_tangent(const double a, const double b, const double c, const double d) {
            return "[a,b,c,d](const double x) { return (std::exp(a*x)-std::exp(-b*x))/(std::exp(c*x)+std::exp(-d*x)); }";
        };
        static std::string s_softplus() {
            return "[](const double x) { return std::log(1.0+std::exp(x)); }";
        };
        static std::string s_hyperbolic_tangent() {
            return "[](const double x) { return std::tanh(x); }";
        };

    public:
        static ActivationFunction getFunction(std::string name, const std::vector<double>& consts = {}) {
            auto it = aliases.find(name);
            if (it == aliases.end())
                throw std::runtime_error("Activator: unknown activation function");
            name = it->second;

            auto it2 = constants.find(name);
            if (it2 == constants.end()) {
                if (!consts.empty())
                    throw std::runtime_error("Activator: wrong number of constants");
            } else if (it2->second != consts.size())
                throw std::runtime_error("Activator: wrong number of constants");

            if (name == "binary-step")
                return f_binary_step();
            else if (name == "exponential-linear-unit")
                return f_exponential_linear_unit(consts[0]);
            else if (name == "gaussian-error-linear-unit")
                return f_gaussian_error_linear_unit();
            else if (name == "identity")
                return f_identity();
            else if (name == "leaky-rectified-linear-unit")
                return f_leaky_rectified_linear_unit(consts[0]);
            else if (name == "parametric-rectified-linear-unit")
                return f_parametric_rectified_linear_unit(consts[0]);
            else if (name == "rectified-linear-unit")
                return f_rectified_linear_unit();
            else if (name == "scaled-exponential-linear-unit")
                return f_scaled_exponential_linear_unit(consts[0], consts[1]);
            else if (name == "sigmoid")
                return f_sigmoid();
            else if (name == "sigmoid-linear-unit")
                return f_sigmoid_linear_unit();
            else if (name == "smoothed-hyperbolic-tangent")
                return f_smoothed_hyperbolic_tangent(consts[0], consts[1], consts[2], consts[3]);
            else if (name == "softplus")
                return f_softplus();
            else if (name == "hyperbolic-tangent")
                return f_hyperbolic_tangent();

            throw std::runtime_error("Activator: unknown activation function");
        };

        static std::string getString(std::string name, const std::vector<double>& consts = {}) {
            auto it = aliases.find(name);
            if (it == aliases.end())
                throw std::runtime_error("Activator: unknown activation function");
            name = it->second;

            auto it2 = constants.find(name);
            if (it2 == constants.end()) {
                if (!consts.empty())
                    throw std::runtime_error("Activator: wrong number of constants");
            } else if (it2->second != consts.size())
                throw std::runtime_error("Activator: wrong number of constants");

            if (name == "binary-step")
                return s_binary_step();
            else if (name == "exponential-linear-unit")
                return s_exponential_linear_unit(consts[0]);
            else if (name == "gaussian-error-linear-unit")
                return s_gaussian_error_linear_unit();
            else if (name == "identity")
                return s_identity();
            else if (name == "leaky-rectified-linear-unit")
                return s_leaky_rectified_linear_unit(consts[0]);
            else if (name == "parametric-rectified-linear-unit")
                return s_parametric_rectified_linear_unit(consts[0]);
            else if (name == "rectified-linear-unit")
                return s_rectified_linear_unit();
            else if (name == "scaled-exponential-linear-unit")
                return s_scaled_exponential_linear_unit(consts[0], consts[1]);
            else if (name == "sigmoid")
                return s_sigmoid();
            else if (name == "sigmoid-linear-unit")
                return s_sigmoid_linear_unit();
            else if (name == "smoothed-hyperbolic-tangent")
                return s_smoothed_hyperbolic_tangent(consts[0], consts[1], consts[2], consts[3]);
            else if (name == "softplus")
                return s_softplus();
            else if (name == "hyperbolic-tangent")
                return s_hyperbolic_tangent();

            throw std::runtime_error("Activator: unknown activation function");
        };
};

const std::unordered_map<std::string, std::string> Activator::aliases = Activator::ParseAliases({
    { "binary-step", { "binary step", "binary", "step" } },
    { "exponential-linear-unit", { "exponential linear unit", "elu" } },
    { "gaussian", { "gaussian" } },
    { "gaussian-error-linear-unit", { "gaussian error linear unit", "gelu" } },
    { "identity", { "identity", "linear" } },
    { "leaky-rectified-linear-unit", { "leaky rectified linear unit", "leaky relu" } },
    { "parametric-rectified-linear-unit", { "parametric rectified linear unit", "prelu" } },
    { "rectified-linear-unit", { "rectified linear unit", "relu" } },
    { "scaled-exponential-linear-unit", { "scaled exponential linear unit", "selu" } },
    { "sigmoid", { "sigmoid", "logistic", "soft step" } },
    { "sigmoid-linear-unit", { "sigmoid linear unit", "silu", "swish" } },
    { "smoothed-hyperbolic-tangent", { "smoothed hyperbolic tangent", "smht" } },
    { "softplus", { "softplus" } },
    { "hyperbolic-tangent", { "hyperbolic tangent", "tanh" } }
});

const std::unordered_map<std::string, int> Activator::constants = {
    { "exponential-linear-unit", 1 },
    { "leaky-rectified-linear-unit", 1 },
    { "parametric-rectified-linear-unit", 1 },
    { "scaled-exponential-linear-unit", 2 },
    { "smoothed-hyperbolic-tangent", 4 },
};