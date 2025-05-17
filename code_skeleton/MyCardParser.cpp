#include "MyCardParser.hpp"
#include <iostream>
#include <cstdint>

namespace sevens {

void MyCardParser::read_cards(const std::string& /*filename*/) {
    // Initialiser le deck de 52 cartes
    // Les couleurs (suits) sont : 0 = Trèfle, 1 = Carreau, 2 = Coeur, 3 = Pique
    // Les rangs (ranks) sont : 1 = As, 2, 3, ..., 10, 11 = Valet, 12 = Dame, 13 = Roi

    for (uint64_t suit = 0; suit < 4; ++suit) {
        for (uint64_t rank = 1; rank <= 13; ++rank) {
            Card card{static_cast<int>(suit), static_cast<int>(rank)};
            cards_hashmap[suit * 13 + (rank - 1)] = card;
        }
    }

    // Vérification de l'initialisation
    std::cout << "[MyCardParser::read_cards] Deck de 52 cartes initialisé avec succès.\n";
}

} // namespace sevens
