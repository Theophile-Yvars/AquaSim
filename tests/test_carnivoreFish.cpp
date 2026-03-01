#include <gtest/gtest.h>
#include "carnivoreFish.hpp"

class CarnivoreFishTest: public::testing::Test{
protected:
    std::unique_ptr<CarnivoreFish> carnivoreFish;

    void SetUp() override{
        carnivoreFish = std::make_unique<CarnivoreFish>(50, 0);
    }

    void TearDown() override{

    }
};

TEST_F(CarnivoreFishTest, getLife){
    EXPECT_EQ(carnivoreFish->getLife(), 50);
}

TEST_F(CarnivoreFishTest, getAge){
    EXPECT_EQ(carnivoreFish->getAge(), 0);
}

TEST_F(CarnivoreFishTest, addAge){
    EXPECT_EQ(carnivoreFish->getAge(), 0);
    carnivoreFish->addAge();
    EXPECT_EQ(carnivoreFish->getAge(), 1);
    carnivoreFish->addAge();
    EXPECT_EQ(carnivoreFish->getAge(), 2);
}

TEST_F(CarnivoreFishTest, setLife){
    carnivoreFish->setLife(70);
    EXPECT_EQ(carnivoreFish->getLife(), 70);    
    carnivoreFish->setLife(-6);
    EXPECT_EQ(carnivoreFish->getLife(), 0);   
    carnivoreFish->setLife(170);
    EXPECT_EQ(carnivoreFish->getLife(), 100);   
}

TEST_F(CarnivoreFishTest, isDead){
    EXPECT_FALSE(carnivoreFish->isDead());
    carnivoreFish->setLife(0);
    EXPECT_TRUE(carnivoreFish->isDead());
    carnivoreFish->setLife(10);
    EXPECT_FALSE(carnivoreFish->isDead());
    for(int i = 0; i < 20; i++){
        carnivoreFish->addAge();
    }
    EXPECT_EQ(carnivoreFish->getAge(), 20);
    EXPECT_TRUE(carnivoreFish->isDead());
}

TEST_F(CarnivoreFishTest, getType){
    EXPECT_EQ(carnivoreFish->getType(), AgentType::Carnivore);
}