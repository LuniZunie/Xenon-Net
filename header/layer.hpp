#pragma once

#include <iterator>
#include <optional>
#include <vector>

#include "configuration.hpp"
#include "population.hpp"
#include "network.hpp"
#include "neuron.hpp"
#include "synapse.hpp"

class Layer {
    private:
        const Population& population;
        const Network& network;
        NetworkScope& scope;

        int id, depth;

    public:
        struct ImportExport { int index, depth; };

        Layer(const Population& pop, const Network& net, NetworkScope& scp) :
            population(pop), network(net), scope(scp),
            id(scope.registry.add(0x1)), depth(-1) { };

        int get_id() const { return id; };
        int get_index() const { return network.get_index(); };
        int get_size() const { return scope.neurons[this].size(); };

        int get_depth() const;
        void set_depth(const int d);

        Neuron add_neuron(const int h) const;

        void prime() const;
        void update(const Neuron::Update type) const;

        const ImportExport _export() const;

        void destruct();
};