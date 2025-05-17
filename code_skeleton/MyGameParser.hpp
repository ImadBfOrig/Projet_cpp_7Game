#pragma once
#include "Generic_game_parser.hpp"
#include <unordered_map>
#include <cstdint>

namespace sevens {

/**
 * Class for parsing the game layout and initial setup.
 */
class MyGameParser : public Generic_game_parser {
public:
    MyGameParser() = default;
    ~MyGameParser() = default;

    void read_game(const std::string& filename) override;
    void read_cards(const std::string& filename) override;


    // Getter pour accéder au layout
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, bool>>& get_table_layout();
};

} // namespace sevens
