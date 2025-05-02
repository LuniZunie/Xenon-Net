#include "../header/activator.hpp"

const auto ActivatorSearch::_aliases = ActivatorSearch::parse_aliases({
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

const std::unordered_map<std::string, int> ActivatorSearch::_constants = {
    { "exponential-linear-unit", 1 },
    { "leaky-rectified-linear-unit", 1 },
    { "parametric-rectified-linear-unit", 1 },
    { "scaled-exponential-linear-unit", 2 },
    { "smoothed-hyperbolic-tangent", 4 },
};