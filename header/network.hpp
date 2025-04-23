#pragma once

#include <cmath>
#include <iterator>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "../typedef/functions.hpp"

#include "../resource/compiler/.hpp"
#include "../resource/random/.hpp"
#include "../resource/range/.hpp"
#include "../resource/registry/.hpp"

#include "configuration.hpp"
#include "population.hpp"
#include "layer.hpp"
#include "neuron.hpp"
#include "synapse.hpp"

struct NetworkScope {
    const Configuration& config;
    Registry<int> registry;

    std::list<Layer&> layers;
    std::unordered_map<const Layer&, std::list<Neuron&>> neurons;
    struct Synapses {
        std::unordered_map<const Neuron&, std::list<Synapse&>> list;
        std::unordered_map<const Neuron&, std::unordered_map<const Neuron&, Synapse&>> source;
        std::unordered_map<const Neuron&, std::unordered_map<const Neuron&, Synapse&>> target;
    } synapses;

    NetworkScope(const Configuration& config) : config(config), registry(), layers(), neurons(), synapses() {};
};

class Network {
    public:
        enum Status { Dead, Alive };

    private:
        const Population& population;
        NetworkScope& scope;

        struct Activator {
            ActivationFunction function;
            std::string string;
        } activator;
        FitnessFunction trainer;
        OutputFunction receiver;

        Registry<int>& networker;
        Compiler& compiler;

        int id, index;

        Status status;

        struct Fitness {
            double sum;
            int count;
        } fitness{ 0, 0 };

        enum Update { InletOutlet };
        void update(const Update type) const;

    public:
        struct ImportExport {
            int index;
            int fitCount; double fitSum;
            ImportExport(const int i, const Fitness& fit) : index(i), fitCount(fit.count), fitSum(fit.sum) { };
        };
        struct Group : NetworkIndex {
            Group(int g, int i) : NetworkIndex(g, i) { };
        };

        Network(
            const Population& pop, NetworkScope scp,
            ActivationFunction activatorFN, std::string activatorSTR,
            FitnessFunction trainerFN,
            OutputFunction receiverFN,
            Registry<int>& reg, Compiler& cmp,
            const int i
        ) :
            population(pop), scope(scp),
            activator({ activatorFN, activatorSTR }),
            trainer(trainerFN),
            receiver(receiverFN),
            networker(reg), compiler(cmp),
            id(reg.add(0x0)), index(i) { };

        int get_id() const;
        int get_index() const;
        const Group get_group() const;
        int get_size() const;

        Status get_status() const;
        void set_status(const Status s);

        double get_fitness() const;

        Layer add_layer(const int d) const;

        void clear();
        void init();
        void clone_from(const Network& other);
        void evolve();

        void prime() const;
        std::string get_code() const;
        std::string compile(const bool dbg = false) const;
        void input(const std::vector<double>& inputs);

        void _import(const ImportExport data);
        const ImportExport _export() const;

        void del();
};