#pragma once
#include "iagent.hpp"
#include <vector>
#include <memory>

class Cell{
    public:
    Cell(int h, int l);
    std::vector<std::unique_ptr<IAgent>>& getAgents();
    void addAgent(std::unique_ptr<IAgent> agent);
    void reproduction();

    private:
    std::vector<std::unique_ptr<IAgent>> m_agents;
    int m_depth;
    int m_lenght;
    int m_light;
};