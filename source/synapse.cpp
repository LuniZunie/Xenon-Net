#include "../header/synapse.hpp"

int Synapse::get_id() const { return id; };
int Synapse::get_index() const { return network.get_index(); };

const Neuron& Synapse::get_source() const { return source; };
const Neuron& Synapse::get_target() const { return target; };

double Synapse::get_weight() const { return weight; };
void Synapse::set_weight(const double w) {
    const auto range = scope.config.synapse.weight;
    if (range.outside(w))
        throw std::invalid_argument("Synapse: weight out of range.");
    weight = w;
};
double Synapse::mod_weight(const double n) {
    const auto range = scope.config.synapse.weight;
    weight = std::clamp(weight + n, range.get_min(), range.get_max());
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