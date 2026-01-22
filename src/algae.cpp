#include "algae.hpp"


Algae::Algae(int life, int age) : m_life(life), m_age(age){}

void Algae::update()
{
    m_age += 1;
}

int Algae::getLife()
{
    return m_life;
}

void Algae::setLife(int life)
{
    m_life = life;
    if(m_life > 100){
        m_life = 100;
    }
}

void Algae::addAge()
{
    m_age += 1;
}

bool Algae::isDead()
{
    if(m_age >= 10) return true;
    return false;
}

AgentType Algae::getType() const
{
    return AgentType::Algae;
}
