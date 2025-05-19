#include "MyGameMapper.hpp"
#include "RandomStrategy.hpp"
#include "GreedyStrategy.hpp"
#include "SmartMinimaxStrategy.hpp"  // 🔄 Inclusion de la stratégie intelligente
#include <iostream>
#include <memory>

int main() {
    sevens::MyGameMapper gameMapper;

    std::cout << "===== Chargement des cartes =====" << std::endl;
    gameMapper.read_cards("");
    
    std::cout << "\n===== Initialisation du plateau =====" << std::endl;
    gameMapper.read_game("");
    
    std::cout << "\n===== Verification des strategies enregistrees =====" << std::endl;

    // Enregistrement des stratégies
    auto randomStrategy = std::make_shared<sevens::RandomStrategy>();
    auto greedyStrategy = std::make_shared<sevens::GreedyStrategy>();
    auto smartMinimax = std::make_shared<sevens::SmartMinimaxStrategy>();

    gameMapper.registerStrategy(1, smartMinimax);    // SmartMinimax pour le joueur 1
    gameMapper.registerStrategy(2, smartMinimax);  // Greedy pour le joueur 2

    std::cout << "\n===== Simulation silencieuse =====" << std::endl;
    auto results_silent = gameMapper.compute_game_progress(2);
    for (const auto& [playerID, score] : results_silent) {
        std::cout << "Joueur " << playerID << " a termine avec " << score << " cartes restantes.\n";
    }

    std::cout << "\n===== Simulation detaillee =====" << std::endl;
    auto results_detailed = gameMapper.compute_and_display_game(2);
    for (const auto& [playerID, score] : results_detailed) {
        std::cout << "Joueur " << playerID << " a termine avec " << score << " cartes restantes.\n";
    }

    return 0;
}
