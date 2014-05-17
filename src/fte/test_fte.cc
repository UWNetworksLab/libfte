#include <regex>

#include "gtest/gtest.h"

#include "fte/fte.h"
#include "fte/ranking/sample_dfas.h"


TEST(SanityCheckMatch, Test1) {
  std::string str = "aaaaaaaaaaaaaaaa";
  std::regex rx("^(a|b){16}$");
  bool match = regex_match(str.begin(), str.end(), rx);
  EXPECT_EQ(true, match);
}

TEST(SanityCheckMatch, Test2) {
  std::string str = "bbbbbbbbbbbbbbbb";
  std::regex rx("^(a|b){16}$");
  bool match = regex_match(str.begin(), str.end(), rx);
  EXPECT_EQ(true, match);
}

TEST(SanityCheckNoMatch, Test1) {
  std::string str = "aaaaaaaaaaaaaaaaa";
  std::regex rx("^(a|b){16}$");
  bool match = regex_match(str.begin(), str.end(), rx);
  EXPECT_EQ(false, match);
}

TEST(SanityCheckNoMatch, Test2) {
  std::string str = "aaaaaaaaaaaaaaa";
  std::regex rx("^(a|b){16}$");
  bool match = regex_match(str.begin(), str.end(), rx);
  EXPECT_EQ(false, match);
}

TEST(SanityCheckNoMatch, Test3) {
  std::string str = "c";
  std::regex rx("^(a|b){16}$");
  bool match = regex_match(str.begin(), str.end(), rx);
  EXPECT_EQ(false, match);
}

TEST(SanityCheckNoMatch, Test4) {
  std::string str = "cccccccccccccccc";
  std::regex rx("^(a|b){16}$");
  bool match = regex_match(str.begin(), str.end(), rx);
  EXPECT_EQ(false, match);
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

TEST(FteNormalUsage, FpeTest2) {
  fte::Key K = "00000000000000000000000000000000";
  fte::FTE fteObj(VALID_DFA_5, 128,
                  VALID_DFA_5, 128,
                  K);

  std::string input_plaintext = "a";
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  EXPECT_EQ(input_plaintext, output_plaintext);

  std::regex rx("^[\\x00-\\xFF]{1,128}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
}

TEST(FteNormalUsage, FpeTest3) {
  fte::Key K = "00000000000000000000000000000000";
  fte::FTE fteObj(VALID_DFA_5, 256,
                  VALID_DFA_5, 256,
                  K);

  std::string input_plaintext = "a";
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  EXPECT_EQ(input_plaintext, output_plaintext);
  
  std::regex rx("^[\\x00-\\xFF]{1,256}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
}

TEST(FteNormalUsage, FpeTest4) {
  fte::Key K = "00000000000000000000000000000000";
  fte::FTE fteObj(VALID_DFA_5, 512,
                  VALID_DFA_5, 512,
                  K);

  std::string input_plaintext =
    "a";
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  EXPECT_EQ(input_plaintext, output_plaintext);

  std::regex rx("^[\\x00-\\xFF]{1,512}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
}

TEST(FteNormalUsage, FpeTest5) {
  fte::Key K = "00000000000000000000000000000000";
  fte::FTE fteObj(VALID_DFA_5, 1024,
                  VALID_DFA_5, 1024,
                  K);

  std::string input_plaintext =
    "a";
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  EXPECT_EQ(input_plaintext, output_plaintext);

  std::regex rx("^[\\x00-\\xFF]{1,1024}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
}

TEST(FteNormalUsage, FpeTest6) {
  fte::Key K = "00000000000000000000000000000000";
  fte::FTE fteObj(VALID_DFA_5, 2048,
                  VALID_DFA_5, 2048,
                  K);

  std::string input_plaintext =
    "a";
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  EXPECT_EQ(input_plaintext, output_plaintext);

  std::regex rx("^[\\x00-\\xFF]{1,2048}$");
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

  std::regex rx("^[\\x00-\\xFF]{1,32}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
}
