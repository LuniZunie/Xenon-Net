#include "../header/neuron.hpp"

int Neuron::get_id() const { return id; };
int Neuron::get_index() const { return network.get_index(); };
int Neuron::get_depth() const { return layer.get_depth(); };

int Neuron::get_height() const { return height; };
void Neuron::set_height(const int h) {
    if (h < 0)
        throw std::invalid_argument("Neuron: height must be positive.");

    const int size = layer.get_size();
    if (h >= size)
        throw std::invalid_argument("Neuron: height out of range.");
    height = h;
};

double Neuron::get_bias() const { return bias; };
void Neuron::set_bias(const double b) {
    if (scope.config.neuron.bias.outside(b))
        throw std::invalid_argument("Neuron: bias out of range.");
    bias = b;
};
double Neuron::mod_bias(const double n) {
    bias = math::clamp(bias + n, scope.config.neuron.bias);
    return bias;
};

Synapse Neuron::add_synapse(const Neuron& neuron) const {
    if (neuron.get_id() == id)
        throw std::invalid_argument("Neuron: cannot add synapse to itself.");

    Synapse synapse(population, network, scope, *this, neuron);

    scope.synapses.list[this].push_back(&synapse);
    scope.synapses.source[this].insert_or_assign(&neuron, &synapse);
    scope.synapses.target[&neuron].insert_or_assign(this, &synapse);

    return synapse;
};

void Neuron::update(const Update type) {
    switch (type) {
        case Inlet:
            inlet.clear();
            if (layer.get_depth() == 0)
                inlet.insert(this);
            else
                for (const auto [ neuron, synapse ] : scope.synapses.source[this])
                    inlet.insert(neuron->inlet.begin(), neuron->inlet.end());
            break;
        case Outlet:
            outlet.clear();
            if (layer.get_depth() == layer.get_size() - 1)
                outlet.insert(this);
            else
                for (const auto [ neuron, synapse ] : scope.synapses.target[this])
                    outlet.insert(neuron->outlet.begin(), neuron->outlet.end());
            break;
    }
};
const Neuron::CodeData Neuron::get_code(const std::unordered_map<const Neuron*, const CodeData*>& data, const ActivationFunction& activator) const {
    if (outlet.empty())
        return { };

    const std::string name = "N"+std::to_string(id);
    if (layer.get_depth() == 0)
        return { 2, "const double "+name+"=activator("+std::to_string(bias)+"+std::atof(argv["+std::to_string(height)+"]));", name };
    else if (inlet.empty()) { // no inputs, can calculate beforehand
        int n = bias;
        for (const auto [ neuron, synapse ] : scope.synapses.source[this])
            n += synapse->get_weight() * std::stod(data.at(neuron)->code);
        return { 1, std::to_string(activator(n)), name };
    } else { // has inputs, need to calculate on the fly
        std::string code = "";
        double likeTerms = bias;
        for (const auto [ neuron, synapse ] : scope.synapses.source[this]) {
            const CodeData* datum = data.at(neuron);
            const double weight = synapse->get_weight();
            if (weight != 0) {
                switch (datum->type) {
                    case 1: // parsed
                        likeTerms += weight * std::stod(datum->code);
                        break;
                    case 2: // code
                        code += "+"+name+"*"+std::to_string(weight);
                        break;
                }
            }

            if (likeTerms < 0)
                code += std::to_string(likeTerms);
            else if (likeTerms > 0)
                code += "+"+std::to_string(likeTerms);

            if (code.empty())
                code = "0";
            return { 2, "const double "+name+"=activator("+code+");", name };
        }
    }
};

void Neuron::_import(const ImportExport data) {
    try {
        set_bias(data.bias);
    } catch (...) { throw std::invalid_argument("invalid Neuron import data."); };
};
const Neuron::ImportExport Neuron::_export() const {
    return { network.get_index(), layer.get_depth(), height, bias };
};

void Neuron::destruct(bool byLayer) {
    for (auto synapse : scope.synapses.list[this])
        synapse->destruct(this);

    scope.synapses.list.erase(this);
    scope.synapses.source.erase(this);
    scope.synapses.target.erase(this);

    if (!byLayer)
        scope.neurons[&layer].remove(this);

    scope.registry.erase(0x2, id);
    delete this;
};