#pragma once

#include <optional>
#include <stdexcept>
#include <vector>

#include "../resource/range/.hpp"

struct Configuration {
    struct Mutation { double rate; };
    struct ChangeMutation : Mutation {
        double amount;
        ChangeMutation(double r, double a) : Mutation{r}, amount(a) {};
    };

    struct Population {
        int size = 1e+2;
        int group = 1;
        double equality = 5e-2;
    } population;
    struct Network {
        int inputs;
        int outputs;
        std::optional<std::vector<const int>> hidden = std::nullopt;
        struct Fitness {
            bool inverse = false;
            bool average = false;
        } fitness;
    } network;
    struct Neuron {
        Range<double> bias{1.0};
    } neuron;
    struct Synapse {
        Range<double> weight{1.0};
    } synapse;
    struct Mutate {
        struct Layer {
            Mutation add{6e-3};
            Mutation remove{6e-3};
        } layer;
        struct Neuron {
            Mutation add{1e-2};
            Mutation remove{1e-2};
            ChangeMutation change{4e-2, 1e-1};
        } neuron;
        struct Synapse {
            Mutation add{2e-2};
            Mutation remove{2e-2};
            ChangeMutation change{8e-2, 1e-1};
        } synapse;
    } mutate;

    Configuration(const int inputs, const int outputs, const std::optional<std::vector<const int>> hidden = std::nullopt) :
        population(), network{inputs, outputs}, neuron(), synapse(), mutate() {
            if (inputs <= 0 || outputs <= 0)
                throw std::invalid_argument("Configuration: inputs and outputs must be positive.");

            if (hidden.has_value()) {
                const auto& value = hidden.value();
                for (const int& h : value)
                    if (h <= 0)
                        throw std::invalid_argument("Configuration: hidden must be positive.");
                network.hidden = value;
            }
        };
};