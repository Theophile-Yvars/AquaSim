#pragma once
#include "iagent.hpp"

class Algae:public IAgent{
    public:
    Algae(int life, int age);
    void update() override;
    int getLife() override;
    void setLife(int life) override;
    void addAge() override;
    bool isDead() override;
    AgentType getType() const override;

    private:
    int m_life;
    int m_age;
};