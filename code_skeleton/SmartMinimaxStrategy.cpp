#include "SmartMinimaxStrategy.hpp"
#include <algorithm>
#include <iostream>
#include <limits>

namespace sevens {

SmartMinimaxStrategy::SmartMinimaxStrategy() : myID(0) {}

void SmartMinimaxStrategy::initialize(uint64_t playerID) {
    myID = playerID;
}

int SmartMinimaxStrategy::selectCardToPlay(
    const std::vector<Card>& hand,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout
) {
    if (hand.empty()) {
        return -1; // Si la main est vide, le joueur passe son tour
    }

    int bestIndex = -1;
    int bestValue = std::numeric_limits<int>::min();

    // Parcourir toutes les cartes de la main
    for (size_t i = 0; i < hand.size(); ++i) {
        const Card& card = hand[i];

        if (isCardPlayable(card, tableLayout)) {
            // Copie du plateau pour la simulation
            auto simulatedLayout = tableLayout;
            simulatedLayout[card.suit][card.rank] = true;

            // Lancer le minimax pour évaluer le coup
            int value = minimax(hand, simulatedLayout, 2, false);

            if (value > bestValue) {
                bestValue = value;
                bestIndex = static_cast<int>(i);
            }
        }
    }

    // Si aucune carte n'est jouable, on passe le tour
    return bestIndex == -1 ? -1 : bestIndex;
}

void SmartMinimaxStrategy::observeMove(uint64_t playerID, const Card& playedCard) {
    opponentMoves[playerID].push_back(playedCard);
}

void SmartMinimaxStrategy::observePass(uint64_t /*playerID*/) {
    // Rien à faire ici pour l'instant
}

std::string SmartMinimaxStrategy::getName() const {
    return "SmartMinimaxStrategy";
}

bool SmartMinimaxStrategy::isCardPlayable(
    const Card& card,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout
) const {
    // 🔄 Vérification stricte : Le 7 doit être posé pour débloquer la suite
    if (!tableLayout.at(card.suit).count(7)) {
        return false;
    }

    // 🔄 Vérification des voisins immédiats
    bool leftPlayable = (card.rank > 1) && tableLayout.at(card.suit).count(card.rank - 1);
    bool rightPlayable = (card.rank < 13) && tableLayout.at(card.suit).count(card.rank + 1);

    // 🔍 Cas particuliers (1 et 13 ne sont jouables que si le 2 ou le 12 est présent)
    if (card.rank == 1) {
        return tableLayout.at(card.suit).count(2);
    }
    if (card.rank == 13) {
        return tableLayout.at(card.suit).count(12);
    }

    // 🔍 Cas général : les voisins doivent exister
    return leftPlayable || rightPlayable;
}


int SmartMinimaxStrategy::minimax(
    const std::vector<Card>& hand,
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout,
    int depth,
    bool maximizingPlayer
) {
    if (depth == 0) {
        return evaluate(tableLayout);
    }

    if (maximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (const Card& card : hand) {
            if (isCardPlayable(card, tableLayout)) {
                auto simulatedLayout = tableLayout;
                simulatedLayout[card.suit][card.rank] = true;
                int eval = minimax(hand, simulatedLayout, depth - 1, false);
                maxEval = std::max(maxEval, eval);
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const Card& card : hand) {
            if (isCardPlayable(card, tableLayout)) {
                auto simulatedLayout = tableLayout;
                simulatedLayout[card.suit][card.rank] = true;
                int eval = minimax(hand, simulatedLayout, depth - 1, true);
                minEval = std::min(minEval, eval);
            }
        }
        return minEval;
    }
}

int SmartMinimaxStrategy::evaluate(
    const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout
) const {
    int score = 0;

    // Plus il y a de cartes posées dans les séquences, plus le score est élevé
    for (const auto& [suit, ranks] : tableLayout) {
        for (const auto& [rank, isPlaced] : ranks) {
            if (isPlaced) {
                score += 10;
            }
        }
    }

    // Plus l'adversaire est bloqué, plus le score est élevé
    for (const auto& [playerID, moves] : opponentMoves) {
        score -= static_cast<int>(moves.size()) * 5;
    }

    return score;
}

} // namespace sevens
