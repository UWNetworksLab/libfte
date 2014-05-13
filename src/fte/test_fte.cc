#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <regex>

#include "gtest/gtest.h"

#include "fte/fte.h"

#include "fte/ranking/sample_dfas.h"

TEST(SanityCheck, Test1) {
  std::string str = "aaaaaaaaaaaaaaaa";
  std::regex rx("^(a|b){16}$");
  bool match = regex_match(str.begin(), str.end(), rx);
  EXPECT_EQ(true, match);
}

TEST(SanityCheck, Test2) {
  std::string str = "bbbbbbbbbbbbbbbb";
  std::regex rx("^(a|b){16}$");
  bool match = regex_match(str.begin(), str.end(), rx);
  EXPECT_EQ(true, match);
}

TEST(FteNormalUsage, FpeTest1) {
  fte::Key K = "00000000000000000000000000000000";
  fte::FTE fteObj(VALID_DFA_1, 64,
                  VALID_DFA_1, 64,
                  K);

  std::string input_plaintext =
    "aaaabbbbaaaabbbbaaaabbbbaaaabbbbaaaabbbbaaaabbbbaaaabbbbaaaabbbb";
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  EXPECT_EQ(input_plaintext, output_plaintext);

  std::regex rx("^(a|b){1,64}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);

  EXPECT_EQ(true, match);
}

TEST(FteNormalUsage, FteTest1) {
  fte::Key K = "00000000000000000000000000000000";
  fte::FTE fteObj(VALID_DFA_1, 32,
                  VALID_DFA_1, 64,
                  K);

  std::string input_plaintext = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  EXPECT_EQ(input_plaintext, output_plaintext);

  std::regex rx("^(a|b){1,64}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);

  EXPECT_EQ(true, match);
}

TEST(FteNormalUsage, FteTest2) {
  fte::Key K = "00000000000000000000000000000000";
  fte::FTE fteObj(VALID_DFA_1, 32,
                  VALID_DFA_5, 32,
                  K);

  std::string input_plaintext = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  EXPECT_EQ(input_plaintext, output_plaintext);

  std::regex rx("^.{1,32}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);

  EXPECT_EQ(true, match);
}
