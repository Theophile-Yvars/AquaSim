#include "ocean.hpp"
#include "algae.hpp"
#include "herbivoreFish.hpp"
#include "carnivoreFish.hpp"
#include "cell.hpp"

#include <random>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

/* ================= RANDOM ================= */

int Ocean::_randomNumber(int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, max);
    return dist(gen);
}

/* ================= MOVE ALGAE ================= */

void Ocean::_moveAlgae(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    for (auto it = agents.begin(); it != agents.end(); )
    {
        if (dynamic_cast<Algae*>(it->get()))
        {
            if (_randomNumber(99) < 60)
            {
                int ni = static_cast<int>(i);
                int ny = static_cast<int>(y);

                // Déplacer jusqu'à 8 cases
                int steps = _randomNumber(7) + 1; // 1 à 8 cases
                for (int step = 0; step < steps; ++step)
                {
                    int direction = _randomNumber(7);
                    int next_i = ni;
                    int next_y = ny;

                    switch (direction)
                    {
                        case 0: next_i--; break;
                        case 1: next_i--; next_y++; break;
                        case 2: next_y++; break;
                        case 3: next_i++; next_y++; break;
                        case 4: next_i++; break;
                        case 5: next_i++; next_y--; break;
                        case 6: next_y--; break;
                        case 7: next_i--; next_y--; break;
                    }

                    // Vérifier limites
                    if (next_i < 0 || next_i >= (int)m_cells.size() ||
                        next_y < 0 || next_y >= (int)m_cells[ni].size())
                        break; // stop si hors limites

                    ni = next_i;
                    ny = next_y;
                }

                // Déplacer l'algue
                if (ni != (int)i || ny != (int)y)
                {
                    m_cells[ni][ny]->addAgent(std::move(*it));
                    it = agents.erase(it);
                    continue;
                }
            }
        }
        ++it;
    }
}


/* ================= MOVE HERBIVORES ================= */

void Ocean::_moveHerbivor(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    // Vérifier présence d'algues
    bool isAlgaePresent = false;
    for (auto& agent : agents)
    {
        if (dynamic_cast<Algae*>(agent.get()))
        {
            isAlgaePresent = true;
            break;
        }
    }

    for (auto it = agents.begin(); it != agents.end(); )
    {
        if (auto herbi = dynamic_cast<HerbivoreFish*>(it->get()))
        {
            int ni = static_cast<int>(i);
            int ny = static_cast<int>(y);

            if (isAlgaePresent)
            {
                // Déplacement aléatoire d'une case
                if (_randomNumber(99) < 60)
                {
                    int direction = _randomNumber(7);
                    switch (direction)
                    {
                        case 0: ni--; break;
                        case 1: ni--; ny++; break;
                        case 2: ny++; break;
                        case 3: ni++; ny++; break;
                        case 4: ni++; break;
                        case 5: ni++; ny--; break;
                        case 6: ny--; break;
                        case 7: ni--; ny--; break;
                    }
                }
            }
            else
            {
                // Pas d'algues : monter vers le niveau 2
                if (ni > 2)
                    ni--; // monte d'une case vers le haut
            }

            // Déplacement si toujours dans les limites
            if (ni >= 0 && ni < (int)m_cells.size() &&
                ny >= 0 && ny < (int)m_cells[i].size() &&
                (ni != (int)i || ny != (int)y))
            {
                m_cells[ni][ny]->addAgent(std::move(*it));
                it = agents.erase(it);
                continue;
            }
        }

        ++it;
    }
}


/* ================= MOVE CARNIVORES ================= */

void Ocean::_moveCarnivore(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    for (auto it = agents.begin(); it != agents.end(); )
    {
        if (auto carni = dynamic_cast<CarnivoreFish*>(it->get()))
        {
            int ni = static_cast<int>(i);
            int ny = static_cast<int>(y);

            if (carni->getLife() <= 70) // Faim → remonter vers le niveau 2
            {
                for (int step = 0; step < 5; ++step)
                {
                    if (ni <= 2) break; // niveau 2 max
                    // Vérifier herbivores sur la case au-dessus
                    bool herbivorePresent = false;
                    for (auto& agent : m_cells[ni-1][ny]->getAgents())
                        if (dynamic_cast<HerbivoreFish*>(agent.get()))
                            herbivorePresent = true;

                    if (herbivorePresent) break; // stop si herbivore
                    --ni; // monter d'une case
                }
            }
            else if (carni->getLife() >= 90) // Roplu → descendre vers le niveau 18
            {
                for (int step = 0; step < 5; ++step)
                {
                    if (ni >= 18) break; // niveau 18 max
                    ++ni; // descendre d'une case
                }
            }
            else // Déplacement aléatoire normal
            {
                if (_randomNumber(99) < 60)
                {
                    int direction = _randomNumber(7);
                    switch (direction)
                    {
                        case 0: ni--; break;
                        case 1: ni--; ny++; break;
                        case 2: ny++; break;
                        case 3: ni++; ny++; break;
                        case 4: ni++; break;
                        case 5: ni++; ny--; break;
                        case 6: ny--; break;
                        case 7: ni--; ny--; break;
                    }
                }
            }

            // Déplacer si nouveau niveau différent
            if (ni != (int)i || ny != (int)y)
            {
                if (ni >= 0 && ni < (int)m_cells.size() &&
                    ny >= 0 && ny < (int)m_cells[i].size())
                {
                    m_cells[ni][ny]->addAgent(std::move(*it));
                    it = agents.erase(it);
                    continue;
                }
            }
        }

        ++it;
    }
}

/* ================= DEATH ================= */

void Ocean::_deadAgent(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    agents.erase(
        std::remove_if(
            agents.begin(),
            agents.end(),
            [](const std::unique_ptr<IAgent>& agent)
            {
                return agent->isDead();
            }
        ),
        agents.end()
    );
}

/* ================= REPRODUCTION ALGAE ================= */

void Ocean::_reproductionAlgae(size_t i, size_t y)
{
    std::vector<std::unique_ptr<IAgent>> newAgents;

    for (auto& elt : m_cells[i][y]->getAgents())
    {
        if (auto algae = dynamic_cast<Algae*>(elt.get()))
        {
            if (algae->getLife() == 100)
            {
                algae->setLife(50);
                newAgents.push_back(std::make_unique<Algae>(50, 0));
            }
        }
    }

    for (auto& a : newAgents)
        m_cells[i][y]->addAgent(std::move(a));
}

/* ================= REPRODUCTION HERBIVORES ================= */

void Ocean::_reproduceHerbivores(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();
    std::vector<HerbivoreFish*> eligible;

    for (auto& agent : agents)
    {
        if (auto herbi = dynamic_cast<HerbivoreFish*>(agent.get()))
        {
            if (herbi->getLife() >= 80 && herbi->getAge() > 2)
                eligible.push_back(herbi);
        }
    }

    std::vector<std::unique_ptr<IAgent>> newAgents;

    for (size_t j = 0; j + 1 < eligible.size(); j += 2)
    {
        int nbChildren = _randomNumber(4) + 2;

        for (int k = 0; k < nbChildren; k++)
            newAgents.push_back(std::make_unique<HerbivoreFish>(50, 0));
    }

    for (auto& child : newAgents)
        m_cells[i][y]->addAgent(std::move(child));
}

/* ================= REPRODUCTION CARNIVORES ================= */

void Ocean::_reproduceCarnivores(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();
    std::vector<CarnivoreFish*> eligible;

    // 1. Sélection des carnivores éligibles
    for (auto& agent : agents)
    {
        if (auto carni = dynamic_cast<CarnivoreFish*>(agent.get()))
        {
            if (carni->getLife() >= 80 && carni->getAge() >= 7)
                eligible.push_back(carni);
        }
    }

    // 2. Création des enfants
    std::vector<std::unique_ptr<IAgent>> newAgents;

    for (size_t j = 0; j + 1 < eligible.size(); j += 2)
    {
        int nbChildren = _randomNumber(4); // 0 à 4

        for (int k = 0; k < nbChildren; k++)
            newAgents.push_back(std::make_unique<CarnivoreFish>(50, 0));
    }

    // 3. Ajout dans la cellule
    for (auto& child : newAgents)
        m_cells[i][y]->addAgent(std::move(child));
}


/* ================= EAT ================= */

void Ocean::_eat(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    for (auto& elt : agents)
    {
        if (auto algae = dynamic_cast<Algae*>(elt.get()))
        {
            if (i < 10)
                algae->setLife(algae->getLife() + m_cells[i][y]->getLight());
            else
                algae->setLife(algae->getLife() - 30);
        }
    }

    for (auto& elt : agents)
    {
        if (auto herbi = dynamic_cast<HerbivoreFish*>(elt.get()))
        {
            int eaten = 0;

            for (auto it = agents.begin();
                 it != agents.end() && eaten < 20 && herbi->getLife() < 100; )
            {
                if (auto algae = dynamic_cast<Algae*>(it->get()))
                {
                    herbi->setLife(min(100, herbi->getLife() + 1));
                    it = agents.erase(it);
                    eaten++;
                }
                else
                    ++it;
            }

            if (herbi->getLife() == 100)
                herbi->setLife(herbi->getLife() - 20);

            if (eaten == 0)
                herbi->setLife(herbi->getLife() - 15);
        }
    }
}

void Ocean::_eatCarnivor(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    for (auto& elt : agents)
    {
        if (auto carni = dynamic_cast<CarnivoreFish*>(elt.get()))
        {
            // Pas faim → ne chasse pas
            if (carni->getLife() > 70)
                continue;

            bool hasEaten = false;

            for (auto it = agents.begin(); it != agents.end(); )
            {
                if (auto herbi = dynamic_cast<HerbivoreFish*>(it->get()))
                {
                    carni->setLife(std::min(100, carni->getLife() + 50));
                    it = agents.erase(it); // herbivore mangé
                    hasEaten = true;
                    break; // un seul herbivore par tour
                }
                else
                    ++it;
            }

            // Faim mais rien trouvé
            if (!hasEaten)
            {
                carni->setLife(carni->getLife() - 5);
            }
        }
    }
}



/* ================= AGE ================= */

void Ocean::_ageing(size_t i, size_t y)
{
    for (auto& agent : m_cells[i][y]->getAgents())
        agent->addAge();
}

/* ================= OCEAN ================= */

Ocean::Ocean() : m_tour(0)
{
    m_cells.resize(20);

    for (size_t i = 0; i < m_cells.size(); ++i)
    {
        m_cells[i].resize(10);
        for (size_t y = 0; y < m_cells[i].size(); ++y)
            m_cells[i][y] = std::make_unique<Cell>(i, y);
    }
}

void Ocean::update()
{
    display();
    m_tour++;

    for (size_t i = 0; i < m_cells.size(); i++)
    {
        for (size_t y = 0; y < m_cells[i].size(); y++)
        {
            _eat(i, y);
            _eatCarnivor(i,y);
            _reproductionAlgae(i, y);
            _reproduceHerbivores(i, y);
            _reproduceCarnivores(i,y);
            _moveAlgae(i, y);
            _moveHerbivor(i, y);
            _moveCarnivore(i,y);
            _deadAgent(i, y);
            _ageing(i, y);
        }
    }
}

/* ================= DISPLAY ================= */

void Ocean::display()
{
    cout << "\n=== Ocean tour " << m_tour << " ===\n\n";
    const string SEP = " || ";

    for (size_t i = 0; i < m_cells.size(); ++i)
    {
        for (size_t y = 0; y < m_cells[i].size(); ++y)
        {
            int nbPlants = 0, nbFish = 0, nbSharks = 0;

            for (const auto& agent : m_cells[i][y]->getAgents())
            {
                switch (agent->getType())
                {
                    case AgentType::Algae: nbPlants++; break;
                    case AgentType::Herbivore: nbFish++; break;
                    case AgentType::Carnivore: nbSharks++; break;
                }
            }

            cout << "("
                 << setw(5) << nbPlants << "🌱,"
                 << setw(5) << nbFish   << "🐟,"
                 << setw(5) << nbSharks << "🦈)";

            if (y < m_cells[i].size() - 1)
                cout << SEP;
        }
        cout << "\n";
    }
    cout << "\n";
}

/* ================= INIT & RUN ================= */

void Ocean::initAlgae()
{
    for (size_t i = 0; i < m_cells.size(); i++)
    {
        for (size_t y = 0; y < m_cells[i].size(); y++)
        {
            if (i < 20 && _randomNumber(99) < 60)
                m_cells[i][y]->addAgent(make_unique<Algae>(50, 0));
        }
    }
}

void Ocean::initHerbivore()
{
    for (size_t i = 0; i < m_cells.size(); i++)
    {
        for (size_t y = 0; y < m_cells[i].size(); y++)
        {
            if (i >= 5 && i <= 15 && _randomNumber(99) < 60)
                m_cells[i][y]->addAgent(make_unique<HerbivoreFish>(50, 0));
            if (i >= 5 && i <= 15 && _randomNumber(99) < 60)
                m_cells[i][y]->addAgent(make_unique<HerbivoreFish>(50, 0));
            if (i >= 5 && i <= 15 && _randomNumber(99) < 60)
                m_cells[i][y]->addAgent(make_unique<HerbivoreFish>(50, 0));
        }
    }
}

void Ocean::initCarnivore()
{
    for (size_t i = 0; i < m_cells.size(); i++)
    {
        for (size_t y = 0; y < m_cells[i].size(); y++)
        {
            if (i >= 10 && i <= 20 && _randomNumber(99) < 80)
                m_cells[i][y]->addAgent(make_unique<CarnivoreFish>(50, 0));
        }
    }
}

void Ocean::runSimulation(int nbTour)
{
    initAlgae();
    for (int i = 0; i < 10; i++)
        update();
    initHerbivore();
    initCarnivore();
    for (int i = 0; i < nbTour; i++)
        update();
}
