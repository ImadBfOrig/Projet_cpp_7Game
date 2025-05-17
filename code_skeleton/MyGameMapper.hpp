#pragma once
#include "Generic_game_mapper.hpp"
#include "MyCardParser.hpp"
#include "MyGameParser.hpp"
#include "PlayerStrategy.hpp"  // 🔄 Ajouté pour corriger l'erreur
#include <memory>
#include <unordered_map>

namespace sevens {

class MyGameMapper : public Generic_game_mapper {
public:
    MyGameMapper();

    void read_cards(const std::string& filename) override;
    void read_game(const std::string& filename) override;
    bool hasRegisteredStrategies() const;
    void registerStrategy(uint64_t playerID, std::shared_ptr<sevens::PlayerStrategy> strategy);
    std::vector<std::pair<uint64_t, uint64_t>> compute_game_progress(uint64_t numPlayers) override;
    std::vector<std::pair<uint64_t, uint64_t>> compute_and_display_game(uint64_t numPlayers) override;

private:
    std::unique_ptr<MyCardParser> card_parser;
    std::unique_ptr<MyGameParser> game_parser;
    std::unordered_map<uint64_t, std::shared_ptr<sevens::PlayerStrategy>> strategies; // 🔄 Correction du namespace
};

} // namespace sevens
