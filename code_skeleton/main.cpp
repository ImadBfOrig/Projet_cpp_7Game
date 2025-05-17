#include "MyGameMapper.hpp"
#include "RandomStrategy.hpp"
#include "GreedyStrategy.hpp"
#include <iostream>
#include <memory>

int main() {
    sevens::MyGameMapper gameMapper;

    std::cout << "===== Chargement des cartes =====" << std::endl;
    gameMapper.read_cards("");
    
    std::cout << "\n===== Initialisation du plateau =====" << std::endl;
    gameMapper.read_game("");
    
    std::cout << "\n===== Vérification des stratégies enregistrées =====" << std::endl;

    auto randomStrategy = std::make_shared<sevens::RandomStrategy>();
    auto greedyStrategy = std::make_shared<sevens::GreedyStrategy>();

    gameMapper.registerStrategy(1, randomStrategy);
    gameMapper.registerStrategy(2, greedyStrategy);

    std::cout << "\n===== Simulation silencieuse =====" << std::endl;
    auto results_silent = gameMapper.compute_game_progress(2);
    for (const auto& [playerID, score] : results_silent) {
        std::cout << "Joueur " << playerID << " a terminé avec " << score << " cartes restantes.\n";
    }

    return 0;
}
