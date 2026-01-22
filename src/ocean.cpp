#include "ocean.hpp"
#include "algae.hpp"
#include "cell.hpp"
#include <random>
#include <iostream>
#include <iomanip> // pour std::setw
#include <algorithm>

using namespace std;

int Ocean::_randomNumber(int max)
{
    std::random_device rd;      // source d'entropie
    std::mt19937 gen(rd());     // moteur (Mersenne Twister)
    std::uniform_int_distribution<int> dist(0, max); // [0,100]
    return dist(gen);
}

Ocean::Ocean() : m_tour(0)
{
    m_cells.resize(20);
    for (size_t i = 0; i < m_cells.size(); ++i) {
        m_cells[i].resize(10);
        for (size_t y = 0; y < m_cells[i].size(); ++y) {
            m_cells[i][y] = std::make_unique<Cell>(i, y);
        }
    }
}


void Ocean::update() {
    display();
    
    m_tour += 1;

    for(size_t i = 0; i < m_cells.size(); i++) { 
        for(size_t y = 0; y < m_cells[i].size(); y++) {
            // Manger
            for(auto& elt : m_cells[i][y]->getAgents()) {
                if(auto algae = dynamic_cast<Algae*>(elt.get())) {
                    if(i != 10){
                        int gain = 5 * (9-i/9);
                        algae->setLife(algae->getLife() + gain);
                    }
                }
               
            }


            // Reproduction
            std::vector<std::unique_ptr<IAgent>> newAgents;
            for(auto& elt : m_cells[i][y]->getAgents()) {
                if(auto algae = dynamic_cast<Algae*>(elt.get())) {
                    if(algae->getLife() == 100) {
                        algae->setLife(50);
                        newAgents.push_back(std::make_unique<Algae>(50, 0));
                    }
                    elt->addAge();
                }
               
            }

            // Ajouter les nouveaux agents après la boucle
            for(auto& a : newAgents) {
                m_cells[i][y]->addAgent(std::move(a));
            }

            // Moove

            // 60 % de bouger pour une algue
            for(size_t i = 0; i < m_cells.size(); i++) { 
                for(size_t y = 0; y < m_cells[i].size(); y++) {
                    
                }
            }

            // Supprimer les agents morts
            auto& agents = m_cells[i][y]->getAgents();
            agents.erase(
                std::remove_if(agents.begin(), agents.end(),
                    [](const std::unique_ptr<IAgent>& agent){
                        return agent->isDead();
                    }),
                agents.end()
            );

        }
    }
}


void Ocean::display()
{
    std::cout << "=== Ocean tour " << m_tour << " ===\n";

    for (size_t i = 0; i < m_cells.size(); ++i) {
        for (size_t y = 0; y < m_cells[i].size(); ++y) {
            if (!m_cells[i][y]) {
                std::cout << std::setw(12) << "(0🌱,0🐟,0🦈)";
                continue;
            }

            // Compter les agents par type
            int nbPlants = 0;
            int nbFish = 0;
            int nbSharks = 0;

            for (auto& agent : m_cells[i][y]->getAgents()) {
                AgentType type = agent->getType(); // Chaque agent doit avoir une méthode type()
                if (type == AgentType::Algae) nbPlants++;
                else if (type == AgentType::Herbivore) nbFish++;
                else if (type == AgentType::Carnivore) nbSharks++;
            }

            // Affichage formaté pour la cellule
            std::cout << "(" << nbPlants << "🌱," << nbFish << "🐟," << nbSharks << "🦈)";
        }
        std::cout << "\n"; // ligne suivante
    }
    std::cout << "\n";
}

void Ocean::init(){
    for(size_t i = 0; i < m_cells.size() ; i++){ 
        for(size_t y = 0; y < m_cells[i].size(); y++){
            if(i < 10 && _randomNumber(99) < 60){
                m_cells[i][y]->addAgent(make_unique<Algae>(100,0));
            }
        }
    }
}

void Ocean::runSimulation(int nbTour)
{
    init();
    for(int i = 0; i < nbTour; i++){
        update();
    }
}
