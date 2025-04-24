#pragma once

#include <algorithm>
#include <string>
#include <unordered_set>

#include "../typedef/functions.hpp"

#include "../resource/random/.hpp"
#include "../resource/range/.hpp"

#include "configuration.hpp"
#include "population.hpp"
#include "network.hpp"
#include "layer.hpp"
#include "synapse.hpp"

class Neuron {
    private:
        const Population& population;
        const Network& network;
        const Layer& layer;
        NetworkScope& scope;

        int id, height;
        double bias;

    public:
        enum Update { Inlet, Outlet };
        struct ImportExport {
            int index, depth, height;
            double bias;
        };
        struct CodeData {
            int type;
            std::string code, name;
        };

        std::unordered_set<Neuron*> inlet, outlet;

        Neuron(const Population& pop, const Network& net, const Layer& lay, NetworkScope& scp) :
            population(pop), network(net), layer(lay), scope(scp),
            id(scope.registry.add(0x2)), height(-1), bias(Random::generate<double>(scp.config.neuron.bias)) { };

        int get_id() const;
        int get_index() const;
        int get_depth() const;

        int get_height() const;
        void set_height(const int h);

        double get_bias() const;
        void set_bias(const double b);
        double mod_bias(const double n);

        Synapse add_synapse(const Neuron& neuron) const;

        void update(const Update type);
        const CodeData get_code(const std::unordered_map<const Neuron*, const CodeData*>& data, const ActivationFunction& activator) const;

        void _import(const ImportExport data);
        const ImportExport _export() const;

        void del(bool byLayer);
};