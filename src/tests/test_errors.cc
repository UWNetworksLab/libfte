#include "gtest/gtest.h"
#include "ranker.h"
#include "dfas.h"

TEST(CauseException, InvalidFstFormatException1) {
  try {
    ranker rankerObj(INVALID_DFA_1, 16);
    EXPECT_TRUE(false);
  } catch (InvalidFstFormat e) {
  }
}

TEST(CauseException, InvalidFstFormatExceptionAsFteException1) {
  try {
    ranker rankerObj(INVALID_DFA_1, 16);
    EXPECT_TRUE(false);
  } catch (FteException e) {
  }
}

TEST(CauseException, InvalidFstFormatException2) {
  try {
    ranker rankerObj(INVALID_DFA_2, 16);
    EXPECT_TRUE(false);
  } catch (InvalidFstFormat e) {
  }
}

TEST(CauseException, InvalidFstFormatException3) {
  try {
    ranker rankerObj(INVALID_DFA_3, 16);
    EXPECT_TRUE(false);
  } catch (InvalidFstFormat e) {
  }
}

TEST(CauseException, InvalidFstFormatException4) {
  try {
    ranker rankerObj(INVALID_DFA_4, 16);
    EXPECT_TRUE(false);
  } catch (InvalidFstFormat e) {
  }
}

TEST(CauseException, InvalidInputNoAcceptingPathsException1) {
  try {
    ranker rankerObj(INVALID_DFA_5, 16);
    EXPECT_TRUE(false);
  } catch (InvalidInputNoAcceptingPaths e) {
  }
}

TEST(CauseException, InvalidRankInputException) {
  try {
    ranker rankerObj(VALID_DFA_1, 16);
    rankerObj.rank("xxx");
    EXPECT_TRUE(false);
  } catch (InvalidSymbol e) {
  }
}

