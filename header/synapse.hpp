#pragma once

#include <algorithm>
#include <stdexcept>

#include "../module/math/main.hpp"
#include "../module/random/main.hpp"
#include "../module/range/main.hpp"

#include "configuration.hpp"
#include "population.hpp"
#include "network.hpp"
#include "layer.hpp"
#include "neuron.hpp"

class Synapse {
    private:
        const Population& population;
        const Network& network;
        NetworkScope& scope;

        const Neuron& source;
        const Neuron& target;

        int id;
        double weight;

    public:
        struct ImportExport {
            int index, sourceDepth, sourceHeight, targetDepth, targetHeight;
            double weight;

            ImportExport(const Neuron& source, const Neuron& target, const double w) :
                index(source.get_index()),
                sourceDepth(source.get_depth()), sourceHeight(source.get_height()),
                targetDepth(target.get_depth()), targetHeight(target.get_height()),
                weight(w) { };
        };

        Synapse(const Population& pop, const Network& net, NetworkScope& scp, const Neuron& src, const Neuron& tgt) :
            population(pop), network(net), scope(scp),
            source(src), target(tgt),
            id(scope.registry.add(0x3)),
            weight(Random::generate<double>(scp.config.synapse.weight)) { };

        int get_id() const;
        int get_index() const;

        int get_source_depth() const;
        int get_source_height() const;
        int get_target_depth() const;
        int get_target_height() const;

        const Neuron& get_source() const;
        const Neuron& get_target() const;

        double get_weight() const;
        void set_weight(const double w);
        double mod_weight(const double n);

        void _import(const ImportExport data);
        const ImportExport _export() const;

        void destruct(Neuron* by = nullptr);
};