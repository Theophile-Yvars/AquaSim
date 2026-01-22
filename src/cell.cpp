#include "cell.hpp"
#include "algae.hpp"

using namespace std;

Cell::Cell(int h, int l)
{
    m_lenght = l;
    m_depth = h;
    m_light = (500-h) / 5;
}

std::vector<std::unique_ptr<IAgent>>& Cell::getAgents()
{
    return m_agents;
}

void Cell::addAgent(std::unique_ptr<IAgent> agent)
{   
    m_agents.push_back(std::move(agent));
}

void Cell::reproduction()
{
    std::vector<std::unique_ptr<IAgent>> newAgents;

    for (auto& agent : m_agents) {
        if (auto algae = dynamic_cast<Algae*>(agent.get())) {
            if (algae->getLife() == 100) {
                algae->setLife(50);
                newAgents.push_back(std::make_unique<Algae>(50, 0));
            }
        }
    }

    // Ajouter tous les nouveaux agents **après la boucle**
    for (auto& newAgent : newAgents) {
        m_agents.push_back(std::move(newAgent));
    }
}
