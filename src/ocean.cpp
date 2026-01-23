#include "ocean.hpp"
#include "algae.hpp"
#include "herbivoreFish.hpp"
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
                int direction = _randomNumber(7);
                int ni = static_cast<int>(i);
                int ny = static_cast<int>(y);

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

/* ================= MOVE HERBIVORES ================= */

void Ocean::_moveHerbivor(size_t i, size_t y)
{
    auto& agents = m_cells[i][y]->getAgents();

    bool isAlgaePresent = false;
    for (auto& agent : agents)
    {
        if (dynamic_cast<Algae*>(agent.get()))
        {
            isAlgaePresent = true;
            break;
        }
    }

    if (isAlgaePresent)
        return;

    for (auto it = agents.begin(); it != agents.end(); )
    {
        if (dynamic_cast<HerbivoreFish*>(it->get()))
        {
            if (_randomNumber(99) < 60)
            {
                int direction = _randomNumber(7);
                int ni = static_cast<int>(i);
                int ny = static_cast<int>(y);

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
            _reproductionAlgae(i, y);
            _reproduceHerbivores(i, y);
            _moveAlgae(i, y);
            _moveHerbivor(i, y);
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

void Ocean::init()
{
    for (size_t i = 0; i < m_cells.size(); i++)
    {
        for (size_t y = 0; y < m_cells[i].size(); y++)
        {
            if (i < 10 && _randomNumber(99) < 60)
                m_cells[i][y]->addAgent(make_unique<Algae>(50, 0));

            if (i >= 5 && i <= 15 && _randomNumber(99) < 40)
                m_cells[i][y]->addAgent(make_unique<HerbivoreFish>(50, 0));
        }
    }
}

void Ocean::runSimulation(int nbTour)
{
    init();
    for (int i = 0; i < nbTour; i++)
        update();
}
