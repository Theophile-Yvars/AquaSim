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