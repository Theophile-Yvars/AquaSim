#pragma once
#include "cell.hpp"
#include "algae.hpp"

class Ocean{
    private:
    int m_tour;
    std::vector<std::vector<std::unique_ptr<Cell>>> m_cells;
    int _randomNumber(int max);

    public:
    Ocean();
    void update();
    void display();
    void init();
    void runSimulation(int nbTour);
};