#if HAVE_CXX11
#include <regex>
#endif

#include "gtest/gtest.h"

#include "fte/fte.h"
#include "fte/ranking/sample_dfas.h"


#if HAVE_CXX11
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
#endif

TEST(FteNormalUsage, FpeTest1) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_1, 64,
                      VALID_DFA_1, 64);

  std::string input_plaintext =
    "aaaabbbbaaaabbbbaaaabbbbaaaabbbbaaaabbbbaaaabbbbaaaabbbbaaaabbbb";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);

#if HAVE_CXX11
  std::regex rx("^(a|b){1,64}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
#endif
}

TEST(FteNormalUsage, FpeTest2) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_5, 128,
                      VALID_DFA_5, 128);

  std::string input_plaintext = "a";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);

#if HAVE_CXX11
  std::regex rx("^[\\x00-\\xFF]{1,128}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
#endif
}

TEST(FteNormalUsage, FpeTest3) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_5, 256,
                      VALID_DFA_5, 256);

  std::string input_plaintext = "a";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);

#if HAVE_CXX11
  std::regex rx("^[\\x00-\\xFF]{1,256}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
#endif
}

TEST(FteNormalUsage, FpeTest4) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_5, 512,
                      VALID_DFA_5, 512);

  std::string input_plaintext = "a";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);

#if HAVE_CXX11
  std::regex rx("^[\\x00-\\xFF]{1,512}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
#endif
}

TEST(FteNormalUsage, FpeTest5) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_5, 1024,
                      VALID_DFA_5, 1024);

  std::string input_plaintext = "a";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);

#if HAVE_CXX11
  std::regex rx("^[\\x00-\\xFF]{1,1024}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
#endif
}

TEST(FteNormalUsage, FpeTest6) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_5, 2048,
                      VALID_DFA_5, 2048);

  std::string input_plaintext = "a";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);

#if HAVE_CXX11
  std::regex rx("^[\\x00-\\xFF]{1,2048}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
#endif
}

TEST(FteNormalUsage, FteTest1) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_1, 32,
                      VALID_DFA_1, 64);

  std::string input_plaintext = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);

#if HAVE_CXX11
  std::regex rx("^(a|b){1,64}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
#endif
}

TEST(FteNormalUsage, FteTest2) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_1, 32,
                      VALID_DFA_5, 32);

  std::string input_plaintext = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);

#if HAVE_CXX11
  std::regex rx("^[\\x00-\\xFF]{1,32}$");
  bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);
  EXPECT_EQ(true, match);
#endif
}

// This test is in support of Issue#10:
//   https://github.com/uProxy/libfte/issues/10
TEST(FteMultipleEncrypts, FteTest1) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_1, 32,
                      VALID_DFA_5, 32);

  std::string input_plaintext = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);
}

// This test is in support of Issue#10:
//   https://github.com/uProxy/libfte/issues/10
TEST(FteMultipleDecrypts, FteTest1) {
  std::string K = "00000000000000000000000000000000";
  fte::Fte fteObj;
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_1, 32,
                      VALID_DFA_5, 32);

  std::string input_plaintext = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  EXPECT_EQ(input_plaintext, output_plaintext);
}
