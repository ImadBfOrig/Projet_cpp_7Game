#include "GreedyStrategy.hpp"
#include <algorithm>
#include <iostream>

namespace sevens {

void GreedyStrategy::initialize(uint64_t playerID) {
    myID = playerID;
}

int GreedyStrategy::selectCardToPlay(
    const std::vector<Card>& hand,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout
) {
    if (hand.empty()) {
        return -1; // passe son tour
    }

    for (size_t i = 0; i < hand.size(); ++i) {
        const Card& card = hand[i];

        // 🔍 Vérification stricte :
        // - Le 7 est jouable si pas encore posé
        if (card.rank == 7 && !tableLayout.at(card.suit).count(7)) {
            return static_cast<int>(i);
        }

        // - Les autres cartes ne sont jouables que si le 7 est posé
        if (!tableLayout.at(card.suit).count(7)) {
            continue;
        }

        // 🔍 Vérification des voisins
        bool leftPlayable = (card.rank > 1) && tableLayout.at(card.suit).count(card.rank - 1);
        bool rightPlayable = (card.rank < 13) && tableLayout.at(card.suit).count(card.rank + 1);

        if (leftPlayable || rightPlayable) {
            return static_cast<int>(i); // Jouer la carte si elle est jouable
        }
    }

    // Si aucune carte n'est jouable, passer le tour
    return -1;
}

void GreedyStrategy::observeMove(uint64_t /*playerID*/, const Card& /*playedCard*/) {}
void GreedyStrategy::observePass(uint64_t /*playerID*/) {}
std::string GreedyStrategy::getName() const {
    return "GreedyStrategy";
}

} // namespace sevens
