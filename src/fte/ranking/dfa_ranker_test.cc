#include "gtest/gtest.h"
#include "regex2dfa.h"

#include "fte/ranking/dfa_ranker.h"

TEST(RankerNormalUsage, Test1) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^(a|b)+$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  std::string input_word;
  mpz_class output_rank;
  rankerObj.Unrank(0, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  EXPECT_EQ(0, output_rank.get_ui());
}

TEST(RankerNormalUsage, Test2) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^(abc)|(abc123)$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  std::string input_word;
  mpz_class output_rank;
  rankerObj.Unrank(0, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  EXPECT_EQ(0, output_rank.get_ui());
}

TEST(RankerNormalUsage, Test3) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^static(a|b)+$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  std::string input_word;
  mpz_class output_rank;
  rankerObj.Unrank(0, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  EXPECT_EQ(0, output_rank.get_ui());
}

TEST(RankerNormalUsage, Test4) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^\\x00\\C+$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  std::string input_word;
  mpz_class output_rank;
  rankerObj.Unrank(0, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  EXPECT_EQ(0, output_rank.get_ui());
}

TEST(RankerNormalUsage, Test5) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^.+$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  std::string input_word;
  mpz_class output_rank;
  rankerObj.Unrank(0, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  EXPECT_EQ(0, output_rank.get_ui());
}

TEST(RankerNormalUsage, Test6) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^(a|b)+$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}

TEST(RankerNormalUsage, Test7) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^(abc)|(abc123)$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}

TEST(RankerNormalUsage, Test8) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^static(a|b)+$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}

TEST(RankerNormalUsage, Test9) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^\\x00\\C+$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}

TEST(RankerNormalUsage, Test10) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa(".+$", &dfa);
  rankerObj.SetLanguage(dfa, 16);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 16, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}


TEST(RankerNormalUsage, Test11) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa(".+$", &dfa);
  rankerObj.SetLanguage(dfa, 32);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 32, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}


TEST(RankerNormalUsage, Test12) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa(".+$", &dfa);
  rankerObj.SetLanguage(dfa, 64);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 32, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}


TEST(RankerNormalUsage, Test13) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa(".+$", &dfa);
  rankerObj.SetLanguage(dfa, 128);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 128, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}


TEST(RankerNormalUsage, Test14) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa(".+$", &dfa);
  rankerObj.SetLanguage(dfa, 256);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 256, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}


TEST(RankerNormalUsage, Test15) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa(".+$", &dfa);
  rankerObj.SetLanguage(dfa, 512);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 512, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}


TEST(RankerNormalUsage, Test16) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa(".+$", &dfa);
  rankerObj.SetLanguage(dfa, 1024);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 1024, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}


TEST(RankerNormalUsage, Test17) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa(".+$", &dfa);
  rankerObj.SetLanguage(dfa, 2048);
  mpz_class words_in_language;
  rankerObj.WordsInLanguage(0, 2048, &words_in_language);
  std::string input_word;
  mpz_class   output_rank;
  rankerObj.Unrank(words_in_language - 1, &input_word);
  rankerObj.Rank(input_word, &output_rank);
  mpz_class expected_rank = words_in_language - 1;
  EXPECT_EQ(expected_rank.get_str(), output_rank.get_str());
}


TEST(ErrorTest, InvalidFstFormat1) {

  fte::ranking::DfaRanker rankerObj;
  bool success = rankerObj.SetLanguage("", 16);
  EXPECT_FALSE(success);
}

TEST(ErrorTest, InvalidFstFormat2) {
  fte::ranking::DfaRanker rankerObj;
  bool success = rankerObj.SetLanguage("0", 16);
  EXPECT_FALSE(success);
}

TEST(ErrorTest, InvalidFstFormat3) {
  fte::ranking::DfaRanker rankerObj;
  bool success = rankerObj.SetLanguage("abcdefg", 16);
  EXPECT_FALSE(success);
}

TEST(ErrorTest, InvalidFstFormat4) {
  fte::ranking::DfaRanker rankerObj;
  bool success = rankerObj.SetLanguage("0	1	0	0", 16);
  EXPECT_FALSE(success);
}

TEST(ErrorTest, InvalidInputNoAcceptingPaths1) {
  fte::ranking::DfaRanker rankerObj;
  std::string invalid_dfa = "0	1	0	0\n"
                            "X";
  bool success = rankerObj.SetLanguage(invalid_dfa, 16);
  EXPECT_FALSE(success);
}

TEST(ErrorTest, InvalidRankInput) {
  fte::ranking::DfaRanker rankerObj;
  std::string dfa;
  regex2dfa::Regex2Dfa("^(a|b)+$", &dfa);  
  rankerObj.SetLanguage(dfa, 16);
  mpz_class rank;
  bool success = rankerObj.Rank("xxx", &rank);
  EXPECT_FALSE(success);
}

TEST(ErrorTest, NoLanguageSet) {
  fte::ranking::DfaRanker rankerObj;
  mpz_class rank = 0;
  bool success = rankerObj.Rank("xxx", &rank);
  EXPECT_EQ(rank.get_ui(), 0);
  EXPECT_FALSE(success);
}

