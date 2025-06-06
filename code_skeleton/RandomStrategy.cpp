#include "RandomStrategy.hpp"
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
#include <iostream>

namespace sevens {

RandomStrategy::RandomStrategy() {
    auto seed = static_cast<unsigned long>(
        std::chrono::system_clock::now().time_since_epoch().count()
    );
    rng.seed(seed);
}

void RandomStrategy::initialize(uint64_t playerID) {
    myID = playerID;
}

int RandomStrategy::selectCardToPlay(
    const std::vector<Card>& hand,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout)
{
    (void)tableLayout;  // 🔄 Ajout pour éviter l'erreur de paramètre inutilisé

    if (hand.empty()) {
        return -1; // pass
    }

    std::uniform_int_distribution<int> dist(0, static_cast<int>(hand.size()) - 1);
    return dist(rng);
}

void RandomStrategy::observeMove(uint64_t /*playerID*/, const Card& /*playedCard*/) {}

void RandomStrategy::observePass(uint64_t /*playerID*/) {}

std::string RandomStrategy::getName() const {
    return "RandomStrategy";
}

} // namespace sevens
