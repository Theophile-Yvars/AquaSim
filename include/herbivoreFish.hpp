#pragma once
#include "iagent.hpp"

class HerbivoreFish : public IAgent{
    public:
    HerbivoreFish(int life, int age);
    int getLife() override;
    void setLife(int life) override;
    void addAge() override;
    int getAge() override;
    bool isDead() override;
    AgentType getType() const override;

    private:
    int m_life;
    int m_age;
};