#include "../header/population.hpp"

Network Population::new_network(const int index) {
    return Network(
        *this, NetworkScope(config),
        _activator.function, _activator.string,
        _trainer,
        _receiver,
        networker, compiler,
        index
    );
};
Network Population::add_network(const int index) {
    Network network = new_network(index);
    networks.push_back(network);
    return network;
};

Population::Status Population::status() const { return _status; };
int Population::generation() const { return statistics.generation; };

int Population::size() const { return networks.size(); };
int Population::alive() const { return statistics.alive; };
int Population::dead() const { return statistics.dead; };

Population::NetworkStat Population::best(std::string type) const {
    if (type == "all")
        return statistics.best.all;
    else if (type == "gen")
        return statistics.best.gen;
    else
        throw std::invalid_argument("Population::best: invalid type");
};
Population::NetworkStat Population::worst(std::string type) const {
    if (type == "all")
        return statistics.worst.all;
    else if (type == "gen")
        return statistics.worst.gen;
    else
        throw std::invalid_argument("Population::worst: invalid type");
};

void Population::activator(const std::string name, const std::vector<double> consts) {
    _activator.function = ActivatorSearch::get_function(name, consts);
    _activator.string = ActivatorSearch::get_string(name, consts);
};
void Population::trainer(FitnessFunction fn) { _trainer = fn; };
void Population::sender(InputFunction fn) { _sender = fn; };
void Population::receiver(OutputFunction fn) { _receiver = fn; };

void Population::reset() { statistics = { }; };

void Population::start() {
    if (_status != OFF)
        throw std::runtime_error("Population: already started.");

    networker.del(0x0);
    networks.clear();

    const int size = config.population.size;
    for (int i = 0; i < size; i++) {
        Network network = add_network(i);
        network.init();
        network.evolve();
        network.prime();
    }

    statistics = { 0, size, 0, { }, { } };
    _status = ON;
};
void Population::pause() {
    if (_status != ON && _status != TRAINING)
        throw std::runtime_error("Population: not started.");

    _status = PAUSED;
};
void Population::resume() {
    if (_status != PAUSED)
        throw std::runtime_error("Population: not paused.");

    if (training != nullptr) {
        training->set_value();
        delete training;
        training = nullptr;
    }

    _status = ON;
};
void Population::stop() {
    if (_status == OFF)
        throw std::runtime_error("Population: not started.");

    _status = OFF;
};
void Population::restart() {
    stop();
    start();
};

template<typename... Args>
int Population::kill(Args... args) {
    static_assert((std::is_same_v<Args, Network::Group> && ...), "All arguments must be of type Network::Group");

    const int size = config.population.size;
    const int groupSize = config.population.group, groupCount = size / groupSize;

    std::vector<Network&> recovery = { };
    try {
        for (const auto& group : { args... }) {
            if (group.group < 0 || group.group >= groupCount)
                throw;
            if (group.index < 0 || group.index >= groupSize)
                throw;

            auto& network = networks[group.group * groupSize + group.index];
            if (network.get_status() == Network::Alive) {
                network.set_status(Network::Dead);
                recovery.push_back(network);
            }
        }
    } catch (...) {
        for (auto& network : recovery)
            network.set_status(Network::Alive);
        throw std::runtime_error("Population::kill: failed to kill networks.");
    }

    const int count = recovery.size();
    statistics.alive -= count;
    statistics.dead += count;

    return count;
};

void Population::train(int iterations, std::optional<int> interval) {
    if (_status != ON)
        throw std::runtime_error("Population: not started.");
    if (iterations <= 0)
        throw std::invalid_argument("Population::train: invalid iterations.");

    _status = TRAINING;
    if (interval.has_value()) {
        const int t = interval.value();
        if (t <= 0)
            throw std::invalid_argument("Population::train: invalid interval.");

        const std::chrono::milliseconds ms(t);
        for (int i = 0; i < iterations; i++) {
            std::this_thread::sleep_for(ms);

            std::vector<std::thread> threads;
            for (auto& network : networks) {
                if (_status != TRAINING) {
                    if (_status == PAUSED) {
                        training = new std::promise<void>();
                        training->get_future().wait();
                    } else if (_status == OFF)
                        return;
                }

                if (network.get_status() == Network::Status::Alive)
                    threads.emplace_back([&network, this]() {
                        network.input(_sender(network.get_group()));
                    });
            }

            for (auto& thread : threads)
                if (thread.joinable())
                    thread.join();
        }
    } else
        for (int i = 0; i < iterations; i++) {
            std::vector<std::thread> threads;
            for (auto& network : networks) {
                if (_status != TRAINING) {
                    if (_status == PAUSED) {
                        training = new std::promise<void>();
                        training->get_future().wait();
                    } else if (_status == OFF)
                        return;
                }

                if (network.get_status() == Network::Status::Alive)
                    threads.emplace_back([&network, this]() {
                        network.input(_sender(network.get_group()));
                    });
            }

            for (auto& thread : threads)
                if (thread.joinable())
                    thread.join();
        }

    _status = ON;
    return;
};
void Population::evolve() {
    if (_status != ON)
        throw std::runtime_error("Population: not started.");

    Network* best = nullptr;
    Network* worst = nullptr;

    double max, min;
    std::vector<std::tuple<Network&, double>> fits;
    for (auto& network : networks) {
        const double fit = network.get_fitness();
        fits.push_back({ network, fit });

        if (best == nullptr) {
            best = &network;
            worst = &network;
            max = min = fit;
        } else if (fit > max) {
            best = &network;
            max = fit;
        } else if (fit < min) {
            worst = &network;
            min = fit;
        }
    }

    statistics.best.gen = { best->get_fitness(), best->get_code() };
    statistics.worst.gen = { worst->get_fitness(), worst->get_code() };

    if (statistics.generation == 0) {
        statistics.best.all = { best->get_fitness(), best->get_code() };
        statistics.worst.all = { worst->get_fitness(), worst->get_code() };
    } else if (statistics.best.gen > statistics.best.all)
        statistics.best.all = { best->get_fitness(), best->get_code() };
    else if (statistics.worst.gen < statistics.worst.all)
        statistics.worst.all = { worst->get_fitness(), worst->get_code() };

    double weight = 0;
    for (auto& [ network, fit ] : fits) {
        double w = Math::norm_one(fit, min, max);
        if (config.network.fitness.inverse)
            w = 1 - w;
        w = Math::denorm_one(w, config.population.equality, 1.0);

        weight += w;
        fit = w;
    }

    networker.del(0x0);

    std::unordered_map<Network&, std::vector<int>> picked = { };
    const int size = config.population.size;
    for (int i = 0; i < size; i++) {
        double rng = Random::gen<double>(Range<double>(0, weight, true, false));
        for (auto& [ network, fit ] : fits) {
            rng -= fit;
            if (rng <= 0) {
                picked[network].push_back(i);
                break;
            }
        }
    }

    std::vector<std::thread> threads;
    std::vector<Network> newNetworks = { };
    for (auto& [ network, indices ] : picked)
        for (const auto& index : indices)
            threads.emplace_back([&newNetworks, &network, index, this]() {
                Network newNetwork = new_network(index);
                newNetwork.clone_from(network);
                newNetwork.evolve();
                newNetwork.prime();
                newNetworks.push_back(newNetwork);
            });
            

    for (auto& thread : threads)
        if (thread.joinable())
            thread.join();

    networks = newNetworks;

    statistics.generation++;
    statistics.alive = size;
    statistics.dead = 0;

    return;
};