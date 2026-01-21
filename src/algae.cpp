#include "algae.hpp"


Algae::Algae(int life, int age) : m_life(life), m_age(age){}

void Algae::update()
{
    m_age += 1;
}

void Algae::reproduction()
{
    if(m_life >= 100){
        
    }
}
