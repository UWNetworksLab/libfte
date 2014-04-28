#include "gtest/gtest.h"
#include "ranker.h"
#include "dfas.h"

TEST(NormalUsage, Test1) {
  ranker rankerObj(VALID_DFA_1, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class   C = rankerObj.rank(X);
  EXPECT_EQ(0, C);
}
