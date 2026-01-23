#include "carnivoreFish.hpp"

CarnivoreFish::CarnivoreFish(int life, int age)
    : m_life(life), m_age(age) {}

int CarnivoreFish::getLife() { return m_life; }

void CarnivoreFish::setLife(int life) {
    m_life = life;
    if (m_life > 100) m_life = 100;
    if (m_life < 0) m_life = 0;
}

int CarnivoreFish::getAge() { return m_age; }

void CarnivoreFish::addAge() { m_age++; }

bool CarnivoreFish::isDead() {
    return m_life <= 0 || m_age >= 20;
}

AgentType CarnivoreFish::getType() const {
    return AgentType::Carnivore;
}
