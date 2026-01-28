#include <gtest/gtest.h>
#include "algae.hpp"

// Fixture = classe de test
class AlgaeTest : public ::testing::Test {
protected:
    std::unique_ptr<Algae> algae;

    void SetUp() override {
        algae = std::make_unique<Algae>(10,0);
    }

    void TearDown() override {
        // nettoyage si besoin
    }
};

TEST_F(AlgaeTest, getLife) {
    EXPECT_EQ(algae->getLife(), 10);
}

TEST_F(AlgaeTest, getAge) {
    EXPECT_EQ(algae->getAge(), 0);
}

TEST_F(AlgaeTest, setLife) {
    algae->setLife(50);
    EXPECT_EQ(algae->getLife(), 50);
    algae->setLife(150);
    EXPECT_EQ(algae->getLife(), 100);
    algae->setLife(-6);
    EXPECT_EQ(algae->getLife(), 0);
}

TEST_F(AlgaeTest, addAge) {
    algae->addAge();
    EXPECT_EQ(algae->getAge(), 1);
    algae->addAge();
    EXPECT_EQ(algae->getAge(), 2);
    algae->addAge();
    EXPECT_EQ(algae->getAge(), 3);
    algae->addAge();
    EXPECT_EQ(algae->getAge(), 4);
}

TEST_F(AlgaeTest, isDead) {
    algae->addAge(); // 1
    algae->addAge(); // 2
    algae->addAge(); // 3
    algae->addAge(); // 4
    algae->addAge(); // 5
    algae->addAge(); // 6
    algae->addAge(); // 7
    algae->addAge(); // 8
    algae->addAge(); // 9
    algae->setLife(10);
    EXPECT_EQ(algae->isDead(), false);
    algae->setLife(0);
    EXPECT_EQ(algae->isDead(), true);
    algae->setLife(10);
    EXPECT_EQ(algae->isDead(), false);
    algae->addAge(); // 10
    EXPECT_EQ(algae->getAge(), 10);
    EXPECT_EQ(algae->isDead(), true);
}

TEST_F(AlgaeTest, getType) {
    EXPECT_EQ(algae->getType(), AgentType::Algae);
}