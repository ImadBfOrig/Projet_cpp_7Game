#include "MyGameParser.hpp"
#include <iterator>
#include <unordered_map>
#include <cstdint>
#include <iostream>

namespace sevens {

void MyGameParser::read_game(const std::string& /*filename*/) {
    // Initialiser le plateau de jeu avec les 7 de chaque couleur
    for (uint64_t suit = 0; suit < 4; ++suit) {
        table_layout[suit][7] = true;
    }

    std::cout << "[MyGameParser::read_game] Plateau de jeu initialisé avec les 7 de chaque couleur.\n";

    // Vérification de l'initialisation
    for (const auto& [suit, ranks] : table_layout) {
        for (const auto& [rank, isPlaced] : ranks) {
            if (isPlaced) {
                std::cout << "Carte posée: Suit = " << suit << ", Rank = " << rank << "\n";
            }
        }
    }
}

void MyGameParser::read_cards(const std::string& /*filename*/) {
    // Implémentation vide car MyGameParser ne lit pas les cartes
}

std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& MyGameParser::get_table_layout() {
    return this->table_layout;
}

} // namespace sevens
