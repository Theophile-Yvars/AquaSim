#pragma once
#include "iagent.hpp"

class Algae:public IAgent{
    public:
    Algae(int life, int age);
    void update() override;
    void reproduction() override;
    private:
    int m_life;
    int m_age;
};