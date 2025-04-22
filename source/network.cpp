#include "../header/network.hpp"

void Network::update(const Update type) const {
    switch (type) {
        case InletOutlet:
            for (auto layer = scope.layers.begin(); layer != scope.layers.end(); layer++)
                (*layer).update(Neuron::Update::Inlet);
            for (auto layer = scope.layers.rbegin(); layer != scope.layers.rend(); layer++)
                (*layer).update(Neuron::Update::Outlet);
            break;
    }
};

int Network::get_id() const { return id; };
int Network::get_index() const { return index; };
const Network::Group Network::get_group() const {
    const int size = scope.config.population.group;
    return { index / size, index % size };
};
int Network::get_size() const { return scope.layers.size(); };

Network::Status Network::get_status() const { return status; };
void Network::set_status(const Status s) { status = s; };

double Network::get_fitness() const {
    if (fitness.count == 0)
        return 0;
    else
        return scope.config.network.fitness.average ?
            fitness.sum / fitness.count :
            fitness.sum;
};

Layer Network::add_layer(const int d) const {
    Layer layer(population, *this, scope);
    layer.set_depth(d);

    scope.layers.insert(std::next(scope.layers.begin(), d), layer);
    return layer;
};

void Network::clear() {
    for (auto& layer : scope.layers)
        layer.del();
    fitness = {0, 0};
};
void Network::init() {
    clear();

    const int inputs = scope.config.network.inputs, outputs = scope.config.network.outputs;

    int i = 0;
    Layer inputLayer = add_layer(i++);
    for (int j = 0; j < inputs; j++)
        inputLayer.add_neuron(j);

    if (scope.config.network.hidden.has_value()) {
        for (const int& h : scope.config.network.hidden.value()) {
            Layer layer = add_layer(i++);
            for (int j = 0; j < h; j++)
                layer.add_neuron(j);
        }
    }

    Layer outputLayer = add_layer(i++);
    for (int j = 0; j < outputs; j++)
        outputLayer.add_neuron(j);
};
void Network::evolve() {
    const bool dynamic = !scope.config.network.hidden.has_value();

    auto& layers = scope.layers;
    if (dynamic) {
        int delta = Random::log<int>(scope.config.mutate.layer.add.rate) - Random::log<int>(scope.config.mutate.layer.remove.rate);
        if (delta > 0) {
            int size = layers.size();
            for (int i = 0; i < delta; i++)
                add_layer(Random::gen<int>(Range<int>(0, size++ - 1, false, false)));
        } else if (delta < 0) {
            delta = std::min(-delta, (int)layers.size() - 2);
            for (int i = 0; i < delta; i++)
                Random::list<Layer&>(layers, 1, 1).del();
        }
    }

    int depth = 0;
    const int depthMax = layers.size() - 1;
    for (auto& layer : layers) {
        auto& neurons = scope.neurons[layer];
        if (dynamic && (depth != 0 || depth != depthMax)) {
            int delta = Random::log<int>(scope.config.mutate.neuron.add.rate) - Random::log<int>(scope.config.mutate.neuron.remove.rate);
            if (delta > 0) {
                int size = neurons.size();
                for (int i = 0; i < delta; i++)
                    layer.add_neuron(Random::gen<int>(Range<int>(0, size++, true, false)));
            } else if (delta < 0) {
                delta = std::min(-delta, (int)neurons.size());
                for (int i = 0; i < delta; i++)
                    Random::list<Neuron&>(neurons).del(false);
            }
        }

        depth++;

        auto otherLayers = layers;
        otherLayers.remove(layer);

        int height = 0;
        for (auto& neuron : neurons) {
            if (Random::condition(scope.config.mutate.neuron.change.rate)) {
                double amount = scope.config.mutate.neuron.change.amount;
                neuron.mod_bias(Random::gen<double>(Range<double>(-amount, amount)));
            }

            int delta = Random::log<int>(scope.config.mutate.synapse.add.rate) - Random::log<int>(scope.config.mutate.synapse.remove.rate);
            if (delta > 0) {
                int size = otherLayers.size();
                for (int i = 0; i < delta; i++) {
                    auto& targetNeurons = scope.neurons[Random::list<Layer&>(otherLayers)];
                    neuron.add_synapse(Random::list<Neuron&>(targetNeurons));
                }
            } else if (delta < 0) {
                auto& synapses = scope.synapses.list[neuron];
                delta = std::min(-delta, (int)synapses.size());
                for (int i = 0; i < delta; i++)
                    Random::list<Synapse&>(synapses).del(&neuron);
            }

            if (depth != 0) {
                auto& sources = scope.synapses.source[neuron];
                for (auto& [ source, synapse ] : sources) {
                    if (Random::condition(scope.config.mutate.synapse.change.rate)) {
                        double amount = scope.config.mutate.synapse.change.amount;
                        synapse.mod_weight(Random::gen<double>(Range<double>(-amount, amount)));
                    }
                }
            }
        }
    }
};

void Network::prime() const {
    int depth = 0;
    for (auto& layer : scope.layers) {
        layer.set_depth(depth++);
        layer.prime();
    }
};
std::string Network::get_code() const {
    prime();

    std::string code = "", rtn = "";
    std::unordered_map<const Neuron&, const Neuron::CodeData&> data;

    auto layers = scope.layers;

    int depth = 0;
    const int depthMax = layers.size() - 1;
    for (const auto& layer : layers) {
        auto& neurons = scope.neurons[layer];
        for (const auto& neuron : neurons) {
            const Neuron::CodeData datum = neuron.get_code(data, this->activator.function);
            data.insert_or_assign(neuron, datum);

            if (depth == depthMax) {
                switch (datum.type) {
                    case 0: // empty
                        rtn += "\tstd::cout << 0 << \" \";\n";
                        break;
                    case 1: // parsed
                        rtn += "\tstd::cout << "+datum.code+" << \" \";\n";
                        break;
                    case 2: // code
                        code += "\t"+datum.code+"\n";
                        rtn += "\tstd::cout << "+datum.name+" << \" \";\n";
                        break;
                }
            } else if (datum.type == 2)
                code += "\t"+datum.code+"\n";
        }

        depth++;
    }

    return  "#include <cstdlib>\n"
            "#include <iostream>\n"
            "\n"
            "const auto activator = "+this->activator.string+";\n"
            "\n"
            "int main(int argc, char* argv[]) {\n"
                +code+
                rtn+
            "   return 0;\n"
            "}";
};
std::string Network::compile(const bool debug) const {
    update(InletOutlet);

    const std::string name = "network-"+std::to_string(id);
    compiler.compile(name, get_code(), debug);

    return name;
};
void Network::input(const std::vector<double>& inputs) {
    if (inputs.size() != scope.config.network.inputs)
        throw std::invalid_argument("Network::input: invalid input size");

    std::string args = "";
    for (const auto& input : inputs)
        args += args.empty() ? std::to_string(input) : " "+std::to_string(input);

    const std::vector<double> output = compiler.execute<double>(std::to_string(id), args);

    const Group group = this->get_group();
    const double fit = this->trainer(group.group, group.index, output);
    fitness.sum += fit, fitness.count++;

    this->reciever(group.group, group.index, output);
};

void Network::_import(const ImportExport data) {
    fitness = { data.fitSum, data.fitCount };
};
const Network::ImportExport Network::_export() const {
    return { index, fitness };
};

void Network::del() {
    for (auto& layer : scope.layers)
        layer.del();

    networker.del(0x0, id);
    compiler.remove("network-"+std::to_string(id));

    delete this;
};