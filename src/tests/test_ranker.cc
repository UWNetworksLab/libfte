#include "gtest/gtest.h"

#include "fte/ranking/dfa.h"

#include "dfas.h"

TEST(RankerNormalUsage, Test1) {
    fte::ranking::dfa rankerObj(VALID_DFA_1, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test2) {
    fte::ranking::dfa rankerObj(VALID_DFA_2, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test3) {
    fte::ranking::dfa rankerObj(VALID_DFA_3, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test4) {
    fte::ranking::dfa rankerObj(VALID_DFA_4, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test5) {
    fte::ranking::dfa rankerObj(VALID_DFA_5, 16);
    std::string X = rankerObj.unrank(0);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(0, C);
}

TEST(RankerNormalUsage, Test6) {
    fte::ranking::dfa rankerObj(VALID_DFA_1, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}

TEST(RankerNormalUsage, Test7) {
    fte::ranking::dfa rankerObj(VALID_DFA_2, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}

TEST(RankerNormalUsage, Test8) {
    fte::ranking::dfa rankerObj(VALID_DFA_3, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}

TEST(RankerNormalUsage, Test9) {
    fte::ranking::dfa rankerObj(VALID_DFA_4, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}

TEST(RankerNormalUsage, Test10) {
    fte::ranking::dfa rankerObj(VALID_DFA_5, 16);
    mpz_class words_in_language = rankerObj.getNumWordsInLanguage( 0, 16 );
    std::string X = rankerObj.unrank(words_in_language-1);
    mpz_class   C = rankerObj.rank(X);
    EXPECT_EQ(words_in_language-1, C);
}
