#include "../header/synapse.hpp"

int Synapse::get_id() const { return id; };
int Synapse::get_index() const { return network.get_index(); };

int Synapse::get_source_depth() const { return source.get_depth(); };
int Synapse::get_source_height() const { return source.get_height(); };
int Synapse::get_target_depth() const { return target.get_depth(); };
int Synapse::get_target_height() const { return target.get_height(); };

const Neuron& Synapse::get_source() const { return source; };
const Neuron& Synapse::get_target() const { return target; };

double Synapse::get_weight() const { return weight; };
void Synapse::set_weight(const double w) {
    if (scope.config.synapse.weight.outside(w))
        throw std::invalid_argument("Synapse: weight out of range.");
    weight = w;
};
double Synapse::mod_weight(const double n) {
    weight = math::clamp(weight + n, scope.config.synapse.weight);
    return weight;
};

void Synapse::_import(const ImportExport data) {
    try {
        set_weight(data.weight);
    } catch (...) { throw std::invalid_argument("invalid Synapse import data"); }
};
const Synapse::ImportExport Synapse::_export() const {
    return { source, target, weight };
};

void Synapse::destruct(Neuron* by) {
    const auto targetPtr = &target, sourcePtr = &source;
    if (by != targetPtr) {
        scope.synapses.list[targetPtr].remove(this);
        scope.synapses.source[targetPtr].erase(sourcePtr);
    } else {
        scope.synapses.list[sourcePtr].remove(this);
        scope.synapses.target[sourcePtr].erase(targetPtr);
    }

    scope.registry.erase(0x3, id);
    delete this;
};