# Projet Sevens - Stratégie "SmartMinimax"

## Informations sur l'équipe
- Binome : BAFOU Imad, SLIMANI Karim  
- Projet : Stratégie pour le jeu Sevens, Projet C++ 2025  

---

## Vue d'ensemble du projet
	
Le but est de développer une stratégie optimisée pour le jeu de cartes Sevens dans un environnement de compétition.  

  Le jeu est simulé en deux modes distincts :  
	- Mode Interne : Le programme embarque les stratégies directement dans le code.
	- Mode Compétition : Les stratégies sont chargées dynamiquement depuis des bibliothèques partagées (".dll" sous Windows).

  Notre stratégie principale, ==> SmartMinimaxStrategy <==, est inspirée de l'algorithme Minimax. L'idée est de simuler plusieurs coups à l'avance pour choisir le meilleur mouvement possible à chaque tour. 
 
  Le projet a été un défi à la fois sur l'optimisation des choix de cartes, mais aussi sur le respect strict des règles de Sevens.

---

## Logique de gestion du jeu

  La gestion du jeu se fait principalement dans le fichier "MyGameMapper.cpp". Nous avons :  

1. Initialisation des cartes :  
   - Les 7 de chaque couleur sont directement placés sur le plateau.
   - Les autres cartes sont mélangées aléatoirement et distribuées aux joueurs (5 cartes chacun).

2. Mise à jour du plateau :  
   - À chaque coup joué, la carte est marquée comme posée dans un tableau associatif ("table_layout").
   - Seules les cartes adjacentes à celles déjà posées peuvent être jouées (ex : si le 7 de Pique est posé, le 6 ou le 8 de Pique peut être joué).

3. Vérification de jouabilité :  
   - Avant chaque coup, une vérification stricte est effectuée pour s'assurer que la carte peut être jouée (sinon le joueur passe son tour).

4. Déroulement des tours :  
   - Les stratégies s'exécutent à tour de rôle, choisissant la meilleure carte à jouer selon leur logique.
   - Le jeu se termine quand plus aucun joueur ne peut jouer.

---

##  Stratégie implémentee : SmartMinimaxStrategy

###  Pourquoi cette stratégie ?

  Le choix de SmartMinimaxStrategy repose sur l'idée que dans Sevens, ANTICIPER les coups adverses est essentiel pour optimiser ses mouvements.  
	- En évitant de débloquer des zones trop tôt (comme le 6 ou le 8 autour des 7), on peut retarder l'adversaire.
	- La stratégie essaie de maximiser ses opportunités de jeu tout en minimisant celles de l'adversaire.

### Comment fonctionne SmartMinimaxStrategy ?

1. Évaluation de l'état actuel :  
   - On récupère les cartes actuellement posées sur le plateau.
   - On identifi les mouvements possibles.

2. Simulations de coups (Minimax) :  
   - Pour chaque carte jouable, on simule les prochains tours jusqu'à une profondeur fixe.
   - L'algorithme Minimax évalue chaque scenario et attribue un score.

3. Heuristique de score :  
   +++ Les mouvements qui augmentent les possibilités de jeu sont favorisés. 
   --- Les mouvements qui débloquent des cartes pour l'adversaire sont pénalisés.

4. Sélection du meilleur coup :  
   - Après la simulation, le meilleur coup est choisi en fonction du score minimax.

---

### Observations :
	- Face à des stratégies simples (RandomStrategy et GreedyStrategy), SmartMinimax surpasse systématiquement les adversaires.
	- Les cartes sont jouées de manière stratégique pour débloquer le moins d'options possibles aux adversaires.
	- Lors d'un duel contre elle-même, elle optimise les choix en cherchant à minimiser le nombre de cartes restantes.

### Limites observées :
	- Manque d'adaptation dynamique : Le Minimax est calculé sur une profondeur fixe. En situation réelle, adapter cette profondeur serait bénéfique.
	- Stratégie trop défensive parfois : Contre une stratégie ultra-agressive, notre stratégie peut être (potentiellement) surprise en fin de partie.
	- Aléatoire non parfait : La distribution initiale est randomisée, mais peut favoriser un joueur sur un tirage chanceux. 

---

##  Améliorations possibles
	- Iterative Deepening : Parcourir l'arbre des possibilités de manière itérative avec des délais fixes.
	- Ajout d'un modèle de prédiction : Prédire les choix adverses sur les prochains tours. (à vrai dire, le but principale était d'intégrer un model IA qui pourrait faire la prediction, mais on avoue avoir eu du mal car nos competences en IA sont principalement en pyhton !!)
	- Dynamic Depth Search : Ajuster dynamiquement la profondeur du Minimax en fonction de l'état du jeu.

---

## Références et Remerciements
- [C++ STL Documentation](https://en.cppreference.com/w/)
- [Minimax Algorithm Explained](https://en.wikipedia.org/wiki/Minimax)
- [HeyCoach Minimax Algorithm in C++](https://blog.heycoach.in/minimax-algorithm-in-c-game-theory/)
- [YouTube Minimax: How Computers Play Games](https://www.youtube.com/watch?v=SLgZhpDsrfc&t=24s)
- [OpenAI Documentation](https://platform.openai.com)
- Cours de programmation avancée de la Sorbonne

---

##  Instructions de Compilation et Exécution
  Dans le terminal de votre choix :

1. 

### 2. En mode Competition :
####  1. Compilation du jeu 
g++ -std=c++17 -Wall -Wextra -O3 -fPIC -DBUILD_SHARED_LIB -shared SmartMinimaxStrategy.cpp -o SmartMinimaxStrategy.dll "-Wl,--out-implib=SmartMinimaxStrategy.lib" 

#### 2. Execution en mode compétition :
./sevens_game competition SmartMinimaxStrategy.dll random_strategy.dll
 

