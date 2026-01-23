#pragma once
#include "cell.hpp"
#include "algae.hpp"

class Ocean{
    private:
    int m_tour;
    std::vector<std::vector<std::unique_ptr<Cell>>> m_cells;
    int _randomNumber(int max);
    void _moveAlgae(size_t i, size_t y);
    void _moveHerbivor(size_t i, size_t y);
    void _moveCarnivore(size_t i, size_t y);
    void _deadAgent(size_t i, size_t y);
    void _reproductionAlgae(size_t i, size_t y);
    void _reproduceHerbivores(size_t i, size_t y);
    void _reproduceCarnivores(size_t i, size_t y);
    void _eat(size_t i, size_t y);
    void _eatCarnivor(size_t i, size_t y);
    void _ageing(size_t i, size_t y);

    public:
    Ocean();
    void update();
    void display();
    void initAlgae();
    void initHerbivore();
    void initCarnivore();
    void runSimulation(int nbTour);
};