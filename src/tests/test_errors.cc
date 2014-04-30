#include "gtest/gtest.h"
#include "fte/ranker.h"
#include "dfas.h"

TEST(CauseException, InvalidFstFormatException1) {
    try {
        fte::ranker rankerObj(INVALID_DFA_1, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidFstFormat e) {
    }
}

TEST(CauseException, InvalidFstFormatExceptionAsFteException1) {
    try {
        fte::ranker rankerObj(INVALID_DFA_1, 16);
        EXPECT_TRUE(false);
    } catch (fte::FteException e) {
    }
}

TEST(CauseException, InvalidFstFormatException2) {
    try {
        fte::ranker rankerObj(INVALID_DFA_2, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidFstFormat e) {
    }
}

TEST(CauseException, InvalidFstFormatException3) {
    try {
        fte::ranker rankerObj(INVALID_DFA_3, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidFstFormat e) {
    }
}

TEST(CauseException, InvalidFstFormatException4) {
    try {
        fte::ranker rankerObj(INVALID_DFA_4, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidFstFormat e) {
    }
}

TEST(CauseException, InvalidInputNoAcceptingPathsException1) {
    try {
        fte::ranker rankerObj(INVALID_DFA_5, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidInputNoAcceptingPaths e) {
    }
}

TEST(CauseException, InvalidRankInputException) {
    try {
        fte::ranker rankerObj(VALID_DFA_1, 16);
        rankerObj.rank("xxx");
        EXPECT_TRUE(false);
    } catch (fte::InvalidSymbol e) {
    }
}

