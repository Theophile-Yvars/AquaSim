#include "cell.hpp"

Cell::Cell(int h, int l)
{
    m_lenght = l;
    m_depth = h;
    m_light = (500-h) / 5;
}

std::vector<IAgent> Cell::getAgents()
{
    return m_agents;
}

void Cell::addAgent(IAgent agent)
{   
    m_agents.push_back(agent);
}
