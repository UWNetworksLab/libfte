#include "gtest/gtest.h"

#include "fte/ranking/dfa_ranker.h"
#include "fte/ranking/sample_dfas.h"

TEST(RankerNormalUsage, Test1) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_1, 16);
  std::string X;
  mpz_class C;
  rankerObj.Unrank(0, &X);
  rankerObj.Rank(X, &C);
  EXPECT_EQ(0, C.get_ui());
}

TEST(RankerNormalUsage, Test2) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_2, 16);
  std::string X;
  mpz_class C;
  rankerObj.Unrank(0, &X);
  rankerObj.Rank(X, &C);
  EXPECT_EQ(0, C.get_ui());
}

TEST(RankerNormalUsage, Test3) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_3, 16);
  std::string X;
  mpz_class C;
  rankerObj.Unrank(0, &X);
  rankerObj.Rank(X, &C);
  EXPECT_EQ(0, C.get_ui());
}

TEST(RankerNormalUsage, Test4) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_4, 16);
  std::string X;
  mpz_class C;
  rankerObj.Unrank(0, &X);
  rankerObj.Rank(X, &C);
  EXPECT_EQ(0, C.get_ui());
}

TEST(RankerNormalUsage, Test5) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 16);
  std::string X;
  mpz_class C;
  rankerObj.Unrank(0, &X);
  rankerObj.Rank(X, &C);
  EXPECT_EQ(0, C.get_ui());
}

TEST(RankerNormalUsage, Test6) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_1, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}

TEST(RankerNormalUsage, Test7) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_2, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}

TEST(RankerNormalUsage, Test8) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_3, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}

TEST(RankerNormalUsage, Test9) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_4, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}

TEST(RankerNormalUsage, Test10) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}


TEST(RankerNormalUsage, Test11) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 32);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 32, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}


TEST(RankerNormalUsage, Test12) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 64);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 32, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}


TEST(RankerNormalUsage, Test13) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 128);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 128, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}


TEST(RankerNormalUsage, Test14) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 256);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 256, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}


TEST(RankerNormalUsage, Test15) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 512);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 512, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}


TEST(RankerNormalUsage, Test16) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 1024);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 1024, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}


TEST(RankerNormalUsage, Test17) {
  fte::ranking::DfaRanker rankerObj;
  rankerObj.SetLanguage(VALID_DFA_5, 2048);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 2048, &words_in_language);
  std::string X;
  mpz_class   C;
  rankerObj.Unrank(words_in_language - 1, &X);
  rankerObj.Rank(X, &C);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), C.get_str());
}


TEST(CauseException, InvalidFstFormatException1) {

    fte::ranking::DfaRanker rankerObj;
    bool success = rankerObj.SetLanguage(INVALID_DFA_1, 16);
    EXPECT_FALSE(success);
}

TEST(CauseException, InvalidFstFormatExceptionAsFteException1) {
    fte::ranking::DfaRanker rankerObj;
    bool success = rankerObj.SetLanguage(INVALID_DFA_1, 16);
    EXPECT_FALSE(success);
}

TEST(CauseException, InvalidFstFormatException2) {
    fte::ranking::DfaRanker rankerObj;
    bool success = rankerObj.SetLanguage(INVALID_DFA_2, 16);
    EXPECT_FALSE(success);
}

TEST(CauseException, InvalidFstFormatException3) {
    fte::ranking::DfaRanker rankerObj;
    bool success = rankerObj.SetLanguage(INVALID_DFA_3, 16);
    EXPECT_FALSE(success);
}

TEST(CauseException, InvalidFstFormatException4) {
    fte::ranking::DfaRanker rankerObj;
    bool success = rankerObj.SetLanguage(INVALID_DFA_4, 16);
    EXPECT_FALSE(success);
}

TEST(CauseException, InvalidInputNoAcceptingPathsException1) {
    fte::ranking::DfaRanker rankerObj;
    bool success = rankerObj.SetLanguage(INVALID_DFA_5, 16);
    EXPECT_FALSE(success);
}

TEST(CauseException, InvalidRankInputException) {
    fte::ranking::DfaRanker rankerObj;
    bool success = rankerObj.SetLanguage(VALID_DFA_1, 16);
    mpz_class rank;
    rankerObj.Rank("xxx", &rank);
    EXPECT_FALSE(success);
}

