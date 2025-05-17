#include "GreedyStrategy.hpp"
#include <algorithm>
#include <iostream>

namespace sevens {

void GreedyStrategy::initialize(uint64_t playerID) {
    myID = playerID;
}

int GreedyStrategy::selectCardToPlay(
    const std::vector<Card>& hand,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout)
{
    (void)tableLayout;  // 🔄 Ajout pour éviter l'erreur de paramètre inutilisé

    if (hand.empty()) {
        return -1; // pass
    }

    return 0; // Toujours choisir la première carte de la main
}

void GreedyStrategy::observeMove(uint64_t /*playerID*/, const Card& /*playedCard*/) {}

void GreedyStrategy::observePass(uint64_t /*playerID*/) {}

std::string GreedyStrategy::getName() const {
    return "GreedyStrategy";
}

} // namespace sevens
