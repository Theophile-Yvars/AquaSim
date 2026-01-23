#include "ocean.hpp"
#include "algae.hpp"
#include "cell.hpp"
#include <random>
#include <iostream>
#include <iomanip> // pour std::setw
#include <algorithm>

using namespace std;

struct Move {
    size_t fromX, fromY;
    size_t toX, toY;
    std::unique_ptr<IAgent> agent;
};

int Ocean::_randomNumber(int max)
{
    std::random_device rd;      // source d'entropie
    std::mt19937 gen(rd());     // moteur (Mersenne Twister)
    std::uniform_int_distribution<int> dist(0, max); // [0,100]
    return dist(gen);
}

void Ocean::_moveAlgae(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    for (auto it = agents.begin(); it != agents.end(); ) {

        if (dynamic_cast<Algae*>(it->get())) {  // plus de variable inutile

            if (_randomNumber(99) < 60) {

                int direction = _randomNumber(7);
                int ni = static_cast<int>(i);
                int ny = static_cast<int>(y);

                switch (direction) {
                    case 0: ni--; break;             // haut
                    case 1: ni--; ny++; break;       // haut droite
                    case 2: ny++; break;             // droite
                    case 3: ni++; ny++; break;       // bas droite
                    case 4: ni++; break;             // bas
                    case 5: ni++; ny--; break;       // bas gauche
                    case 6: ny--; break;             // gauche
                    case 7: ni--; ny--; break;       // haut gauche
                }

                // bornes
                if (ni >= 0 && ni < (int)m_cells.size() &&
                    ny >= 0 && ny < (int)m_cells[i].size()) {

                    m_cells[ni][ny]->addAgent(std::move(*it));
                    it = agents.erase(it);
                    continue;
                }
            }
        }
        ++it;
    }
}


void Ocean::_deadAgent(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();
    agents.erase(std::remove_if(agents.begin(), agents.end(),[](const std::unique_ptr<IAgent>& agent){
        return agent->isDead();
    }),
    agents.end()
    );
}

void Ocean::_reproduction(size_t i, size_t y)
{
    std::vector<std::unique_ptr<IAgent>> newAgents;
    for(auto& elt : m_cells[i][y]->getAgents()) {
        if(auto algae = dynamic_cast<Algae*>(elt.get())) {
            if(algae->getLife() == 100) {
                algae->setLife(50);
                newAgents.push_back(std::make_unique<Algae>(50, 0));
            }
        }
    }

    // Ajouter les nouveaux agents après la boucle
    for(auto& a : newAgents) {
        m_cells[i][y]->addAgent(std::move(a));
    }
}

void Ocean::_eat(size_t i, size_t y)
{
    for(auto& elt : m_cells[i][y]->getAgents()) {
        if(auto algae = dynamic_cast<Algae*>(elt.get())) {
            if(i < 10){
                int gain = m_cells[i][y]->getLight();
                algae->setLife(algae->getLife() + gain);
            }else{
                algae->setLife(algae->getLife() - 30);
            }
        }
    }
}

void Ocean::_ageing(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    for (auto& agent : agents ) {
        agent->addAge();
    }
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
            _eat(i,y);

            // Reproduction
            _reproduction(i,y);

            // Moove
            _moveAlgae(i,y);

            // Supprimer les agents morts
            _deadAgent(i,y);

            // viellissement
            _ageing(i,y);
        }
    }
}


void Ocean::display()
{
    std::cout << "\n=== Ocean tour " << m_tour << " ===\n\n";

    const std::string SEP = " || ";

    for (size_t i = 0; i < m_cells.size(); ++i) {

        for (size_t y = 0; y < m_cells[i].size(); ++y) {

            int nbPlants = 0;
            int nbFish   = 0;
            int nbSharks = 0;

            // Comptage des agents
            for (const auto& agent : m_cells[i][y]->getAgents()) {
                switch (agent->getType()) {
                    case AgentType::Algae:      nbPlants++; break;
                    case AgentType::Herbivore:  nbFish++;   break;
                    case AgentType::Carnivore:  nbSharks++; break;
                }
            }

            // Affichage d'une cellule (largeur fixe)
            std::cout
                << "("
                << std::setw(5) << nbPlants << "🌱,"
                << std::setw(5) << nbFish   << "🐟,"
                << std::setw(5) << nbSharks << "🦈)"
            ;

            // Séparateur entre cellules (pas après la dernière)
            if (y < m_cells[i].size() - 1)
                std::cout << SEP;
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}


void Ocean::init(){
    for(size_t i = 0; i < m_cells.size() ; i++){ 
        for(size_t y = 0; y < m_cells[i].size(); y++){
            if(i < 10 && _randomNumber(99) < 60){
                m_cells[i][y]->addAgent(make_unique<Algae>(50,0));
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
