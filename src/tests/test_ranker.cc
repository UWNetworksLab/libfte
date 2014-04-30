#include "gtest/gtest.h"
#include "ranker.h"
#include "dfas.h"

//0

TEST(RankerNormalUsage, Test1) {
    fte::ranker rankerObj(VALID_DFA_1, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test2) {
    fte::ranker rankerObj(VALID_DFA_2, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test3) {
    fte::ranker rankerObj(VALID_DFA_3, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test4) {
    fte::ranker rankerObj(VALID_DFA_4, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test5) {
    fte::ranker rankerObj(VALID_DFA_5, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

//capacity

TEST(RankerNormalUsage, Test6) {
    fte::ranker rankerObj(VALID_DFA_1, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}

TEST(RankerNormalUsage, Test7) {
    fte::ranker rankerObj(VALID_DFA_2, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}

TEST(RankerNormalUsage, Test8) {
    fte::ranker rankerObj(VALID_DFA_3, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}

TEST(RankerNormalUsage, Test9) {
    fte::ranker rankerObj(VALID_DFA_4, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}

TEST(RankerNormalUsage, Test10) {
    fte::ranker rankerObj(VALID_DFA_5, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}
