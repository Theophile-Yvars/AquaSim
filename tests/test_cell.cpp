#include <gtest/gtest.h>
#include "cell.hpp"
#include "herbivoreFish.hpp"
#include "algae.hpp"

class CellTest:public::testing::Test{
protected:
    std::unique_ptr<Cell> cell;

    void SetUp() override{
        cell = std::make_unique<Cell>(0,0);
    }

    void TearDown() override{

    }
};

TEST_F(CellTest, getLight){
    EXPECT_EQ(cell->getLight(), 50);

    cell.reset();
    cell = std::make_unique<Cell>(1,0);
    EXPECT_EQ(cell->getLight(), 50);

    cell.reset();
    cell = std::make_unique<Cell>(2,0);
    EXPECT_EQ(cell->getLight(), 40);

    cell.reset();
    cell = std::make_unique<Cell>(3,0);
    EXPECT_EQ(cell->getLight(), 40);

    cell.reset();
    cell = std::make_unique<Cell>(4,0);
    EXPECT_EQ(cell->getLight(), 40);

    cell.reset();
    cell = std::make_unique<Cell>(5,0);
    EXPECT_EQ(cell->getLight(), 40);

    cell.reset();
    cell = std::make_unique<Cell>(6,0);
    EXPECT_EQ(cell->getLight(), 40);

    cell.reset();
    cell = std::make_unique<Cell>(7,0);
    EXPECT_EQ(cell->getLight(), 30);

    cell.reset();
    cell = std::make_unique<Cell>(8,0);
    EXPECT_EQ(cell->getLight(), 30);

    cell.reset();
    cell = std::make_unique<Cell>(9,0);
    EXPECT_EQ(cell->getLight(), 30);

    cell.reset();
    cell = std::make_unique<Cell>(10,0);
    EXPECT_EQ(cell->getLight(), 20);

    cell.reset();
    cell = std::make_unique<Cell>(11,0);
    EXPECT_EQ(cell->getLight(), 20);
}

TEST_F(CellTest, addAgent){
    EXPECT_EQ(cell->getAgents().size(), 0);
    cell->addAgent(std::make_unique<HerbivoreFish>(10,0));
    EXPECT_EQ(cell->getAgents().size(), 1);
}

TEST_F(CellTest, getAgent){
    cell->addAgent(std::make_unique<HerbivoreFish>(10,0));
    EXPECT_EQ(cell->getAgents().size(), 1);
    cell->addAgent(std::make_unique<HerbivoreFish>(10,0));
    EXPECT_EQ(cell->getAgents().size(), 2);
    cell->addAgent(std::make_unique<HerbivoreFish>(10,0));
    EXPECT_EQ(cell->getAgents().size(), 3);

    auto& agents = cell->getAgents();
    agents.erase(agents.begin());
    EXPECT_EQ(cell->getAgents().size(), 2);
}

TEST_F(CellTest, reproduction){
    EXPECT_EQ(cell->getAgents().size(), 0);
    cell->addAgent(std::make_unique<Algae>(100,2));
    EXPECT_EQ(cell->getAgents().size(), 1);
    cell->reproduction();
    EXPECT_EQ(cell->getAgents().size(), 2);
}