#pragma once

#include <string>
#include <vector>

#include "../typedef/functions.hpp"

#include "../resource/compiler/.hpp"
#include "../resource/registry/.hpp"

#include "configuration.hpp"
#include "network.hpp"
#include "layer.hpp"
#include "neuron.hpp"
#include "synapse.hpp"

class Population {
    private:
        enum Status { OFF, PAUSED, TRAINING, ON };
        struct Statistics {
            private:
                struct Extrema {
                    private:
                        struct Network {
                            double fitness = 0;
                            std::string code = "";
                        };

                    public:
                        Network all, gen;
                };

            public:
                int generation = 0, alive = 0, dead = 0;
                Extrema best, worst;
        };

        Status status{ OFF };
        Statistics statistics;

        const Configuration config;
        Registry<int> networker;
        Compiler compiler;

        std::vector<Network&> networks = { };

        Population(Configuration& cfg) :
            config(cfg),
            networker(),
            compiler("network") { };

        void Activator(std::string name);
        void Trainer(FitnessFunction fn);
        void Sender(InputFunction fn);
        void Receiver(OutputFunction fn);
};