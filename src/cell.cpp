#include "cell.hpp"
#include "algae.hpp"

using namespace std;

Cell::Cell(int h, int l)
{
    m_lenght = l;
    m_depth = h;
    switch (h)
    {
    case 0:
        m_light = 50;
        break;
    case 1:
        m_light = 50;
        break;
    case 2:
        m_light = 40;
        break;
    case 3:
        m_light = 40;
        break;
    case 4:
        m_light = 40;
        break;
    case 5:
        m_light = 40;
        break;
    case 6:
        m_light = 40;
        break;
    case 7:
        m_light = 30;
        break;
    case 8:
        m_light = 30;
        break;
    case 9:
        m_light = 30;
        break;
    default:
        m_light = 20;
        break;
    }
}

std::vector<std::unique_ptr<IAgent>>& Cell::getAgents()
{
    return m_agents;
}

void Cell::addAgent(std::unique_ptr<IAgent> agent)
{
    if (!agent) return; // sécurité

    // Compter le nombre d'agents du même type déjà présents
    AgentType type = agent->getType();
    int count = 0;
    for (auto& a : m_agents) {
        if (a->getType() == type) count++;
    }

    // Limite à 1000 par type
    if (count >= 1000) {
        // On ne l'ajoute pas
        return;
    }

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

 int Cell::getLight()
 {
     return m_light;
 }