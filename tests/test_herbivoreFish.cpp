#include <gtest/gtest.h>
#include "herbivoreFish.hpp"

class HerbivoreFishTest: public::testing::Test{
protected:
    std::unique_ptr<HerbivoreFish> herbivoreFish;

    void SetUp() override{
        herbivoreFish = std::make_unique<HerbivoreFish>(10, 0);
    }

    void TearDown() override{

    }
};

TEST_F(HerbivoreFishTest, getLife){
    EXPECT_EQ(herbivoreFish->getLife(), 10);
}

TEST_F(HerbivoreFishTest, setLife){
    herbivoreFish->setLife(70);
    EXPECT_EQ(herbivoreFish->getLife(), 70);
    herbivoreFish->setLife(-6);
    EXPECT_EQ(herbivoreFish->getLife(), 0);
    herbivoreFish->setLife(170);
    EXPECT_EQ(herbivoreFish->getLife(), 100);
}

TEST_F(HerbivoreFishTest, getAge){
    EXPECT_EQ(herbivoreFish->getAge(), 0);
}

TEST_F(HerbivoreFishTest, addAge){
    herbivoreFish->addAge();
    EXPECT_EQ(herbivoreFish->getAge(), 1);
    herbivoreFish->addAge();
    EXPECT_EQ(herbivoreFish->getAge(), 2);
    herbivoreFish->addAge();
    EXPECT_EQ(herbivoreFish->getAge(), 3);
    herbivoreFish->addAge();
    EXPECT_EQ(herbivoreFish->getAge(), 4);
    herbivoreFish->addAge();
    EXPECT_EQ(herbivoreFish->getAge(), 5);
}

TEST_F(HerbivoreFishTest, isDead){
    EXPECT_FALSE(herbivoreFish->isDead());
    herbivoreFish->setLife(0);
    EXPECT_TRUE(herbivoreFish->isDead());
    herbivoreFish->setLife(10);
    EXPECT_FALSE(herbivoreFish->isDead());
    for(int i = 0; i < 5; i++){
        herbivoreFish->addAge();
    }
    EXPECT_EQ(herbivoreFish->getAge(), 5);
    EXPECT_TRUE(herbivoreFish->isDead());
}

TEST_F(HerbivoreFishTest, getType){
    EXPECT_EQ(herbivoreFish->getType(),AgentType::Herbivore);
}