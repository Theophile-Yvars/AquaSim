#include "ocean.hpp"
#include "algae.hpp"
#include "cell.hpp"
#include <random>


Ocean::Ocean()
{
    m_tour = 0;
    // i hauteur : y longueur
    for(int i = 0; i < 1000; i ++){
        for(int y = 0; y < 1000; y ++){
            m_cells[i][y] = Cell(i, y);
        }
    }
}

void Ocean::update(){
    for(Cell cell : m_cells){
        for(IAgent agent : m_cells){
            agent.update();
        }
    }
}

void Ocean::display()
{
}

void Ocean::init(){
    for(int i = 0; i < 1000; i ++){ // hauteur
        for(int y = 0; y = 1000; y++){ // longueur
            m_cells[i][y] = Cell(i, y);
        }
    }
    std::random_device rd;      // source d'entropie
    std::mt19937 gen(rd());     // moteur (Mersenne Twister)
    std::uniform_int_distribution<int> dist(0, 100); // [0,100]
    int nombre;
    for(int i = 0; i < 1000; i ++){ // hauteur
        for(int y = 0; y = 1000; y++){ // longueur
            if(i < 500){
                nombre = dist(gen);
                if(nombre > 60){
                    m_cells[i][y].addAgent(Algea(80, 0));
                }
            }
            m_cells[i][y].setDepth(i);
            m_cells[i][y].setLenght(y);
            m_cells[i][y].initLight();
        }
        if(i == 500) break;
    }
}