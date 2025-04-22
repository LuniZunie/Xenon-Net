#pragma once

#include <chrono>
#include <future>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "../typedef/functions.hpp"

#include "../resource/compiler/.hpp"
#include "../resource/registry/.hpp"

#include "activator.hpp"
#include "configuration.hpp"
#include "network.hpp"
#include "layer.hpp"
#include "neuron.hpp"
#include "synapse.hpp"

class Population {
    private:
        enum Status { OFF, PAUSED, TRAINING, ON };
        struct NetworkStat {
            double fitness = 0;
            std::string code = "";

            bool operator==(const NetworkStat& other) const { return fitness == other.fitness; };
            bool operator!=(const NetworkStat& other) const { return fitness != other.fitness; };
            bool operator>(const NetworkStat& other) const { return fitness > other.fitness; };
            bool operator<(const NetworkStat& other) const { return fitness < other.fitness; };
            bool operator>=(const NetworkStat& other) const { return fitness >= other.fitness; };
            bool operator<=(const NetworkStat& other) const { return fitness <= other.fitness; };
        };
        struct Statistics {
            private:
                struct Extrema {
                    NetworkStat all, gen;
                };

            public:
                int generation = 0, alive = 0, dead = 0;
                Extrema best, worst;
        };

        Status _status{ OFF };
        std::promise<void>* training = nullptr;

        Statistics statistics;

        const Configuration config;
        Registry<int> networker;
        Compiler compiler;

        std::vector<Network> networks;

        struct Activator {
            ActivationFunction function;
            std::string string;
        } _activator{ ActivatorSearch::get_function("sigmoid"), ActivatorSearch::get_string("sigmod") };
        FitnessFunction _trainer{ [](NetworkIndex, std::vector<double>) -> double { return 0.0; } };
        InputFunction _sender{ [](NetworkIndex) -> std::vector<double> { return { }; } };
        OutputFunction _receiver{ [](NetworkIndex, std::vector<double>) { } };

        Network add_network(const int index);

    public:
        Population(const Configuration cfg) :
            config(cfg),
            networker(),
            compiler("network") { };

        Status status() const;
        int generation() const;

        int size() const;
        int alive() const;
        int dead() const;

        NetworkStat best(std::string type) const;
        NetworkStat worst(std::string type) const;

        void activator(std::string name, const std::vector<double> consts);
        void trainer(FitnessFunction fn);
        void sender(InputFunction fn);
        void receiver(OutputFunction fn);

        void reset();

        void start();
        void pause();
        void resume();
        void stop();
        void restart();

        template<typename... Args>
        int kill(Args... args);

        void train(int iterations = 1, std::optional<int> interval = std::nullopt);
        void evolve();
};