#include "gtest/gtest.h"
#include "ranker.h"
#include "dfas.h"

TEST(NormalUsage, Test1) {
  ranker rankerObj(VALID_DFA_1, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}


TEST(NormalUsage, Test2) {
  ranker rankerObj(VALID_DFA_2, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}



TEST(NormalUsage, Test3) {
  ranker rankerObj(VALID_DFA_3, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}



TEST(NormalUsage, Test4) {
  ranker rankerObj(VALID_DFA_4, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}



TEST(NormalUsage, Test5) {
  ranker rankerObj(VALID_DFA_5, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}