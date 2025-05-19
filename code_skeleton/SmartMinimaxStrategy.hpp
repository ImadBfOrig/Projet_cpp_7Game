#pragma once
#include "PlayerStrategy.hpp"
#include "Generic_card_parser.hpp"
#include <unordered_map>
#include <vector>
#include <memory>

namespace sevens {

class SmartMinimaxStrategy : public PlayerStrategy {
public:
    SmartMinimaxStrategy();
    ~SmartMinimaxStrategy() override = default;

    void initialize(uint64_t playerID) override;

    int selectCardToPlay(           //utilise Minimax pour déterminer le meilleur coup.
        const std::vector<Card>& hand,
        const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout) override;

    // mettre à jour les mouvements des autres joueurs.    
    void observeMove(uint64_t playerID, const Card& playedCard) override;
    void observePass(uint64_t playerID) override;
    std::string getName() const override;

private:
    uint64_t myID;
    std::unordered_map<uint64_t, std::vector<Card>> opponentMoves;

    // Minimax et évaluation ( algorithme de décision sur plusieurs niveaux.)
    int minimax(const std::vector<Card>& hand,
                const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout,
                int depth, bool maximizingPlayer);

    // évaluation de l'état du plateau pour chaque simulation.
    int evaluate(const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout) const;

    // Vérification si la carte est jouable
    bool isCardPlayable(const Card& card,
                        const std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& tableLayout) const;
};

} // namespace sevens
