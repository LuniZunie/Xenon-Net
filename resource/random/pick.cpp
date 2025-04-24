#include ".hpp"

template <container Container, container Weights>
auto Random::pick(const Container& container, const Weights& weights, unsigned int l, unsigned int r) -> typename Container::value_type* {
    using T = typename Container::value_type;
    using W = typename Weights::value_type;
    static_assert(std::is_same_v<W numeral> && !std::is_signed_v<W>, "Random::pick: Weights must be unsigned integral type.");

    if (container.empty())
        return nullptr;
    else if (container.size() != weights.size())
        return nullptr;

    r = container.size() - r;
    if (l >= r)
        return nullptr;

    W weight = 0;
    auto end = std::next(weights.begin(), r);
    for (auto it = std::next(weights.begin(), l); it != end; ++it)
        weight += *it;

    if (weight == 0)
        return nullptr;

    W n = generate(0, weight);
    for (auto it = std::next(weights.begin(), l); it != end; ++it) {
        n -= *it;
        if (n < 0)
            return &std::next(container.begin(), std::distance(weights.begin(), it));
    }
};

template <container Container>
auto Random::pick(const Container& container, unsigned int l, unsigned int r) -> typename Container::value_type* {
    using T = typename Container::value_type;
    if (container.empty())
        return nullptr;

    r = container.size() - r;
    if (l >= r)
        return nullptr;

    return &std::next(container.begin(), generate(l, r));
};

auto vec = std::vector<int>{1, 2, 3, 4, 5};
auto& vec2 = vec;
auto test = Random::pick<std::list<std::vector<int>>>({vec2});