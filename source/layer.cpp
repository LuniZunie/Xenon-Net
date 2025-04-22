#include "../header/layer.hpp"

int Layer::get_id() const { return id; };
int Layer::get_index() const { return network.get_index(); };
int Layer::get_size() const { return scope.neurons[*this].size(); };

int Layer::get_depth() const { return depth; };
void Layer::set_depth(const int d) {
    if (d < 0)
        throw std::invalid_argument("Layer: depth must be positive.");

    const std::optional<std::vector<const int>> hidden = scope.config.network.hidden;
    if (hidden.has_value() && d >= hidden.value().size())
        throw std::invalid_argument("Layer: depth out of range.");
    depth = d;
};

Neuron Layer::add_neuron(const int h) const {
    const auto& ref = *this;
    Neuron neuron(population, network, ref, scope);

    neuron.set_height(h);
    scope.neurons[ref].insert(std::next(scope.neurons[ref].begin(), h), neuron);

    return neuron;
};

void Layer::prime() const {
    int h = 0;
    for (auto& neuron : scope.neurons[*this])
        neuron.set_height(h++);
};
void Layer::update(const Neuron::Update type) const {
    for (auto& neuron : scope.neurons[*this])
        neuron.update(type);
};

const Layer::ImportExport Layer::_export() const {
    return { network.get_index(), depth };
};

void Layer::del() {
    const auto& ref = *this;
    for (auto& neuron : scope.neurons[ref])
        neuron.del(true);
    scope.neurons.erase(ref);

    scope.registry.del(0x1, id);
    delete this;
};