#pragma once
#include "cell.hpp"
#include "algae.hpp"

class Ocean{
    private:
    int m_tour;
    Cell m_cells[1000][1000];
    
    public:
    Ocean();
    void update();
    void display();
};