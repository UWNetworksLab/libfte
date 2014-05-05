#include "gtest/gtest.h"

#include "dfas.h"
#include "fte/ranking/dfa.h"

TEST(CauseException, InvalidFstFormatException1) {
    try {
        fte::ranking::dfa rankerObj(INVALID_DFA_1, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidFstFormat e) {
    }
}

TEST(CauseException, InvalidFstFormatExceptionAsFTEException1) {
    try {
        fte::ranking::dfa rankerObj(INVALID_DFA_1, 16);
        EXPECT_TRUE(false);
    } catch (fte::FTEException e) {
    }
}

TEST(CauseException, InvalidFstFormatException2) {
    try {
        fte::ranking::dfa rankerObj(INVALID_DFA_2, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidFstFormat e) {
    }
}

TEST(CauseException, InvalidFstFormatException3) {
    try {
        fte::ranking::dfa rankerObj(INVALID_DFA_3, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidFstFormat e) {
    }
}

TEST(CauseException, InvalidFstFormatException4) {
    try {
        fte::ranking::dfa rankerObj(INVALID_DFA_4, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidFstFormat e) {
    }
}

TEST(CauseException, InvalidInputNoAcceptingPathsException1) {
    try {
        fte::ranking::dfa rankerObj(INVALID_DFA_5, 16);
        EXPECT_TRUE(false);
    } catch (fte::InvalidInputNoAcceptingPaths e) {
    }
}

TEST(CauseException, InvalidRankInputException) {
    try {
        fte::ranking::dfa rankerObj(VALID_DFA_1, 16);
        rankerObj.rank("xxx");
        EXPECT_TRUE(false);
    } catch (fte::InvalidSymbol e) {
    }
}

