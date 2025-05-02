#pragma once

#include <array>
#include <bitset>
#include <climits>
#include <cstring>
#include <functional>
#include <limits>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "population.hpp"
#include "network.hpp"
#include "layer.hpp"
#include "neuron.hpp"
#include "synapse.hpp"

class Storage {
    private:
        template <typename T>
        static constexpr int bitsizeof() { return sizeof(T)*CHAR_BIT; };

        enum ObjectType : short {
            population,
            best,
            worst,
            network,
            layer,
            neuron,
            synapse
        };

        template <typename T>
        static unsigned long long encode(const T n) {
            std::bitset<bitsizeof<T>()> bits;
            std::memcpy(&bits, &n, sizeof(T));
            return bits.to_ullong();
        };
        template <typename T>
        static T decode(const std::bitset<bitsizeof<T>()>& bits) {
            T n;
            std::memcpy(&n, &bits, sizeof(T));
            return n;
        };

        static struct Save {
            static struct Call {
                static const std::vector<unsigned long long> synapse(const Synapse& synapse) {
                    const auto data = synapse._export();
                    return {
                        encode(ObjectType::synapse),
                        encode(data.index),
                        encode(data.sourceDepth),
                        encode(data.sourceHeight),
                        encode(data.targetDepth),
                        encode(data.targetHeight),
                        encode(data.weight)
                    };
                };
                static const std::vector<unsigned long long> neuron(const Neuron& neuron) {
                    const auto data = neuron._export();
                    return {
                        encode<short>(ObjectType::neuron),
                        encode(data.index),
                        encode(data.depth),
                        encode(data.height),
                        encode(data.bias)
                    };
                };
                static const std::vector<unsigned long long> layer(const Layer& layer) {
                    const auto data = layer._export();
                    return {
                        encode<short>(ObjectType::layer),
                        encode(data.index),
                        encode(data.depth)
                    };
                };
                static const std::vector<unsigned long long> network(const Network& network) {
                    const auto data = network._export();
                    return {
                        encode<short>(ObjectType::network),
                        encode(data.index),
                    };
                };
                static const std::vector<unsigned long long> population(const Population& population) {
                    const auto gen = population.generation();
                    return {
                        encode<short>(ObjectType::population),
                        encode(gen)
                    };
                };
            } call;
            static struct Type {
                using population = std::invoke_result_t<decltype(call.population), Population>;
                using network = std::invoke_result_t<decltype(call.network), Network>;
                using layer = std::invoke_result_t<decltype(call.layer), Layer>;
                using neuron = std::invoke_result_t<decltype(call.neuron), Neuron>;
                using synapse = std::invoke_result_t<decltype(call.synapse), Synapse>;
            } type;
        } save;

    public:
        static const std::vector<std::vector<unsigned long long>> save(const Population& population, const std::vector<Network*> networks) {
            using DataVector = std::vector<std::vector<unsigned long long>>;
            DataVector popEN, netEN, layEN, neuEN, synEN;

            popEN.reserve(1);
            netEN.reserve(networks.size());

            popEN.push_back(save.call.population(population));
            for (const auto network : networks) {
                const auto& scope = network->scope;
                netEN.push_back(save.call.network(*network));
                for (const auto layer : scope.layers) {
                    layEN.push_back(save.call.layer(*layer));
                    for (const auto neuron : scope.neurons.at(layer)) {
                        neuEN.push_back(save.call.neuron(*neuron));
                        for (const auto synapse : scope.synapses.list.at(neuron))
                            synEN.push_back(save.call.synapse(*synapse));
                    }
                }
            }

            DataVector data; data.reserve(popEN.size() + netEN.size() + layEN.size() + neuEN.size() + synEN.size());
            data.insert(data.end(), popEN.begin(), popEN.end());
            data.insert(data.end(), netEN.begin(), netEN.end());
            data.insert(data.end(), layEN.begin(), layEN.end());
            data.insert(data.end(), neuEN.begin(), neuEN.end());
            data.insert(data.end(), synEN.begin(), synEN.end());
            data.shrink_to_fit();
            return data;
        };
};