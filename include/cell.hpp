#pragma once
#include "iagent.hpp"
#include <vector>

class Cell{
    public:
    Cell(int h, int l);
    std::vector<IAgent> getAgents();
    void addAgent(IAgent agent);

    private:
    std::vector<IAgent> m_agents;
    int m_depth;
    int m_lenght;
    int m_light;
};