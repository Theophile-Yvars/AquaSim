#include "algae.hpp"

void Algae::update(int light){
    m_life = m_life + m_life*light;
}

void Algae::reproduction(){
    
}

Algea::Algea(int life, int age) : m_life(life), m_age(age){}