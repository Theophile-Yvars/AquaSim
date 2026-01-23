#include "herbivoreFish.hpp"

HerbivoreFish::HerbivoreFish(int life, int age)
    : m_life(life), m_age(age) {}

int HerbivoreFish::getLife() { return m_life; }

void HerbivoreFish::setLife(int life) {
    m_life = life;
    if (m_life > 100) m_life = 100;
    if (m_life < 0) m_life = 0;
}

int HerbivoreFish::getAge() { return m_age; }

void HerbivoreFish::addAge() { m_age++; }

bool HerbivoreFish::isDead() {
    return m_life <= 0 || m_age >= 5;
}

AgentType HerbivoreFish::getType() const {
    return AgentType::Herbivore;
}
