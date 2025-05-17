#include "MyGameMapper.hpp"
#include "PlayerStrategy.hpp"
#include "MyCardParser.hpp"
#include "MyGameParser.hpp"
#include <cstdint>
#include <iostream>
#include <iterator>


namespace sevens {

MyGameMapper::MyGameMapper() {
    // Instanciation des parsers
    this->card_parser = std::make_unique<MyCardParser>();
    this->game_parser = std::make_unique<MyGameParser>();
}

void MyGameMapper::read_cards(const std::string& filename) {
    std::cout << "[MyGameMapper::read_cards] Chargement des cartes..." << std::endl;
    card_parser->read_cards(filename);
    std::cout << "[MyGameMapper::read_cards] Chargement terminé." << std::endl;
}

void MyGameMapper::read_game(const std::string& filename) {
    std::cout << "[MyGameMapper::read_game] Initialisation du plateau de jeu..." << std::endl;
    game_parser->read_game(filename);
    std::cout << "[MyGameMapper::read_game] Plateau de jeu initialisé." << std::endl;
}

bool MyGameMapper::hasRegisteredStrategies() const {
    return !strategies.empty();
}

void MyGameMapper::registerStrategy(uint64_t playerID, std::shared_ptr<PlayerStrategy> strategy) {
    strategies[playerID] = strategy;
    std::cout << "[MyGameMapper::registerStrategy] Stratégie enregistrée pour le joueur " << playerID << "." << std::endl;
}

std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::compute_game_progress(uint64_t numPlayers) {
    (void)numPlayers;
    std::cout << "[MyGameMapper::compute_game_progress] Simulation silencieuse en cours..." << std::endl;

    // Initialiser les mains des joueurs
    std::unordered_map<uint64_t, std::vector<Card>> playerHands;
    uint64_t cardIndex = 0;

    for (auto& [id, strategy] : strategies) {
        for (int i = 0; i < 5; ++i) {
            playerHands[id].push_back(card_parser->get_cards_hashmap().at(cardIndex));
            cardIndex++;
        }
        strategy->initialize(id);
    }

    bool gameOver = false;
    while (!gameOver) {
        gameOver = true;

        for (auto& [id, strategy] : strategies) {
            auto& hand = playerHands[id];
            size_t cardIndex = strategy->selectCardToPlay(hand, game_parser->get_table_layout());

            if (cardIndex < hand.size()) {
                Card playedCard = hand[cardIndex];
                hand.erase(hand.begin() + cardIndex);

                game_parser->get_table_layout()[playedCard.suit][playedCard.rank] = true;
                strategy->observeMove(id, playedCard);

                gameOver = false;
            }
        }
    }

    std::vector<std::pair<uint64_t, uint64_t>> results;
    for (const auto& [id, hand] : playerHands) {
        results.emplace_back(id, hand.size());
    }

    std::cout << "[MyGameMapper::compute_game_progress] Simulation terminée." << std::endl;
    return results;
}

std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::compute_and_display_game(uint64_t numPlayers) {
    (void)numPlayers;
    std::cout << "[MyGameMapper::compute_and_display_game] Simulation détaillée en cours..." << std::endl;

    std::unordered_map<uint64_t, std::vector<Card>> playerHands;
    uint64_t cardIndex = 0;

    for (auto& [id, strategy] : strategies) {
        for (int i = 0; i < 5; ++i) {
            playerHands[id].push_back(card_parser->get_cards_hashmap().at(cardIndex));
            cardIndex++;
        }
        strategy->initialize(id);
    }

    bool gameOver = false;
    while (!gameOver) {
        gameOver = true;

        for (auto& [id, strategy] : strategies) {
            auto& hand = playerHands[id];
            size_t cardIndex = strategy->selectCardToPlay(hand, game_parser->get_table_layout());

            if (cardIndex < hand.size()) {
                Card playedCard = hand[cardIndex];
                hand.erase(hand.begin() + cardIndex);

                game_parser->get_table_layout()[playedCard.suit][playedCard.rank] = true;
                strategy->observeMove(id, playedCard);

                std::cout << "Joueur " << id << " joue : Suit = " << playedCard.suit
                          << ", Rank = " << playedCard.rank << std::endl;

                gameOver = false;
            } else {
                std::cout << "Joueur " << id << " passe son tour." << std::endl;
            }
        }
    }

    std::vector<std::pair<uint64_t, uint64_t>> results;
    for (const auto& [id, hand] : playerHands) {
        results.emplace_back(id, hand.size());
    }

    std::cout << "[MyGameMapper::compute_and_display_game] Simulation terminée." << std::endl;
    return results;
}


} // namespace sevens
