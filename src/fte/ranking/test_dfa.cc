#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include "gtest/gtest.h"

#include "fte/ranking/dfa.h"

#include "fte/ranking/sample_dfas.h"

TEST(RankerNormalUsage, Test1) {
  fte::ranking::DFA rankerObj(VALID_DFA_1, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test2) {
  fte::ranking::DFA rankerObj(VALID_DFA_2, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test3) {
  fte::ranking::DFA rankerObj(VALID_DFA_3, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test4) {
  fte::ranking::DFA rankerObj(VALID_DFA_4, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test5) {
  fte::ranking::DFA rankerObj(VALID_DFA_5, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test6) {
  fte::ranking::DFA rankerObj(VALID_DFA_1, 16);
  mpz_class words_in_language = rankerObj.getNumWordsInLanguage(0, 16);
  std::string X = rankerObj.unrank(words_in_language - 1);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(words_in_language - 1, C);
}

TEST(RankerNormalUsage, Test7) {
  fte::ranking::DFA rankerObj(VALID_DFA_2, 16);
  mpz_class words_in_language = rankerObj.getNumWordsInLanguage(0, 16);
  std::string X = rankerObj.unrank(words_in_language - 1);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(words_in_language - 1, C);
}

TEST(RankerNormalUsage, Test8) {
  fte::ranking::DFA rankerObj(VALID_DFA_3, 16);
  mpz_class words_in_language = rankerObj.getNumWordsInLanguage(0, 16);
  std::string X = rankerObj.unrank(words_in_language - 1);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(words_in_language - 1, C);
}

TEST(RankerNormalUsage, Test9) {
  fte::ranking::DFA rankerObj(VALID_DFA_4, 16);
  mpz_class words_in_language = rankerObj.getNumWordsInLanguage(0, 16);
  std::string X = rankerObj.unrank(words_in_language - 1);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(words_in_language - 1, C);
}

TEST(RankerNormalUsage, Test10) {
  fte::ranking::DFA rankerObj(VALID_DFA_5, 16);
  mpz_class words_in_language = rankerObj.getNumWordsInLanguage(0, 16);
  std::string X = rankerObj.unrank(words_in_language - 1);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(words_in_language - 1, C);
}

// ERROR CASES

TEST(CauseException, InvalidFstFormatException1) {
  try {
    fte::ranking::DFA rankerObj(INVALID_DFA_1, 16);
    EXPECT_TRUE(false);
  } catch(fte::InvalidFstFormat e) {
  }
}

TEST(CauseException, InvalidFstFormatExceptionAsFTEException1) {
  try {
    fte::ranking::DFA rankerObj(INVALID_DFA_1, 16);
    EXPECT_TRUE(false);
  } catch(fte::FTEException e) {
  }
}

TEST(CauseException, InvalidFstFormatException2) {
  try {
    fte::ranking::DFA rankerObj(INVALID_DFA_2, 16);
    EXPECT_TRUE(false);
  } catch(fte::InvalidFstFormat e) {
  }
}

TEST(CauseException, InvalidFstFormatException3) {
  try {
    fte::ranking::DFA rankerObj(INVALID_DFA_3, 16);
    EXPECT_TRUE(false);
  } catch(fte::InvalidFstFormat e) {
  }
}

TEST(CauseException, InvalidFstFormatException4) {
  try {
    fte::ranking::DFA rankerObj(INVALID_DFA_4, 16);
    EXPECT_TRUE(false);
  } catch(fte::InvalidFstFormat e) {
  }
}

TEST(CauseException, InvalidInputNoAcceptingPathsException1) {
  try {
    fte::ranking::DFA rankerObj(INVALID_DFA_5, 16);
    EXPECT_TRUE(false);
  } catch(fte::InvalidInputNoAcceptingPaths e) {
  }
}

TEST(CauseException, InvalidRankInputException) {
  try {
    fte::ranking::DFA rankerObj(VALID_DFA_1, 16);
    rankerObj.rank("xxx");
    EXPECT_TRUE(false);
  } catch(fte::InvalidSymbol e) {
  }
}

