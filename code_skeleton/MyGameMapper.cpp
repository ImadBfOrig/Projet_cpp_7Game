#include "MyGameMapper.hpp"
#include "PlayerStrategy.hpp"
#include "MyCardParser.hpp"
#include "MyGameParser.hpp"
#include <cstdint>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <random> 

namespace sevens {

MyGameMapper::MyGameMapper() : rng(std::random_device{}()), initialDistributionDone(false) {
    this->card_parser = std::make_unique<MyCardParser>();
    this->game_parser = std::make_unique<MyGameParser>();
}

void MyGameMapper::read_cards(const std::string& filename) {
    std::cout << "[MyGameMapper::read_cards] Chargement des cartes..." << std::endl;
    card_parser->read_cards(filename);
    std::cout << "[MyGameMapper::read_cards] Chargement termine." << std::endl;
}

void MyGameMapper::read_game(const std::string& filename) {
    std::cout << "[MyGameMapper::read_game] Initialisation du plateau de jeu..." << std::endl;
    game_parser->read_game(filename);
    std::cout << "[MyGameMapper::read_game] Plateau de jeu initialise." << std::endl;
}

bool MyGameMapper::hasRegisteredStrategies() const {
    return !strategies.empty();
}

void MyGameMapper::registerStrategy(uint64_t playerID, std::shared_ptr<sevens::PlayerStrategy> strategy) {
    strategies[playerID] = strategy;
    std::cout << "[MyGameMapper::registerStrategy] Strategie enregistree pour le joueur " << playerID 
              << " (" << typeid(*strategy).name() << ")" << std::endl;
}

bool MyGameMapper::isCardPlayable(const Card& card) const {
    auto& layout = game_parser->get_table_layout();

    if (!layout.at(card.suit).at(7) && card.rank != 7) {
        return false;
    }

    if (card.rank == 1) {
        return layout.at(card.suit).count(2);
    }
    if (card.rank == 13) {
        return layout.at(card.suit).count(12);
    }

    bool leftPlayable = layout.at(card.suit).count(card.rank - 1);
    bool rightPlayable = layout.at(card.suit).count(card.rank + 1);

    return leftPlayable || rightPlayable;
}

void MyGameMapper::distributeCards() {
    if (!initialDistributionDone) {
        std::vector<Card> allCards;
        for (const auto& [index, card] : card_parser->get_cards_hashmap()) {
            if (card.rank != 7) { 
                allCards.push_back(card);
            }
        }

        std::shuffle(allCards.begin(), allCards.end(), rng);

        uint64_t cardIndex = 0;
        for (auto& [id, strategy] : strategies) {
            for (int i = 0; i < 5; ++i) {
                if (cardIndex < allCards.size()) {
                    initialHands[id].push_back(allCards[cardIndex]);
                    cardIndex++;
                }
            }
            strategy->initialize(id);

            // ✅ Afficher la main initiale
            std::cout << "Main initiale du joueur " << id << " (" << typeid(*strategy).name() << ") : ";
            for (const auto& card : initialHands[id]) {
                std::cout << "[" << card.suit << "," << card.rank << "] ";
            }
            std::cout << std::endl;
        }
        initialDistributionDone = true;
    }
}

std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::compute_game_progress(uint64_t numPlayers) {
    (void)numPlayers;
    std::cout << "[MyGameMapper::compute_game_progress] Simulation silencieuse en cours..." << std::endl;

    distributeCards();
    std::unordered_map<uint64_t, std::vector<Card>> playerHands = initialHands;

    bool gameOver = false;
    while (!gameOver) {
        gameOver = true;

        for (auto& [id, strategy] : strategies) {
            auto& hand = playerHands[id];
            size_t cardIndex = strategy->selectCardToPlay(hand, game_parser->get_table_layout());

            if (cardIndex < hand.size()) {
                Card playedCard = hand[cardIndex];

                if (!isCardPlayable(playedCard)) {
                    std::cerr << "ERREUR : La carte [" << playedCard.suit << "," << playedCard.rank
                              << "] ne devrait pas etre jouable ! Coup annule." << std::endl;
                    continue;
                }

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

    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    std::cout << "[MyGameMapper::compute_game_progress] Simulation terminee." << std::endl;
    return results;
}

std::vector<std::pair<uint64_t, uint64_t>> MyGameMapper::compute_and_display_game(uint64_t numPlayers) {
    (void)numPlayers;
    std::cout << "[MyGameMapper::compute_and_display_game] Simulation detaillee en cours..." << std::endl;

    distributeCards();
    std::unordered_map<uint64_t, std::vector<Card>> playerHands = initialHands;

    bool gameOver = false;
    int turn = 1;

    while (!gameOver) {
        gameOver = true;
        std::cout << "\n=== Tour " << turn++ << " ===" << std::endl;

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

                std::cout << "Main restante : ";
                for (const auto& c : hand) {
                    std::cout << "[" << c.suit << "," << c.rank << "] ";
                }
                std::cout << std::endl;

                gameOver = false;
            } else {
                std::cout << "Joueur " << id << " passe son tour." << std::endl;
            }
        }
    }

    std::cout << "[MyGameMapper::compute_and_display_game] Simulation terminee." << std::endl;
    return {};
}

} // namespace sevens
