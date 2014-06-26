#if HAVE_CXX11
#include <regex>
#endif

#include "gtest/gtest.h"
#include "regex2dfa.h"

#include "fte/fte.h"


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
  std::string dfa;
  regex2dfa::Regex2Dfa("^(a|b)+$", &dfa);
  fteObj.SetLanguages(dfa, 64,
                      dfa, 64);

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
  std::string dfa = "";
  regex2dfa::Regex2Dfa("^.+$", &dfa);
  fteObj.SetLanguages(dfa, 128,
                      dfa, 128);

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
  std::string dfa;
  regex2dfa::Regex2Dfa("^.+$", &dfa);
  fteObj.SetLanguages(dfa, 256,
                      dfa, 256);

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
  std::string dfa;
  regex2dfa::Regex2Dfa("^.+$", &dfa);
  fteObj.SetLanguages(dfa, 512,
                      dfa, 512);

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
  std::string dfa;
  regex2dfa::Regex2Dfa("^.+$", &dfa);
  fteObj.SetLanguages(dfa, 1024,
                      dfa, 1024);

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
  std::string dfa;
  regex2dfa::Regex2Dfa("^.+$", &dfa);
  fteObj.SetLanguages(dfa, 2048,
                      dfa, 2048);

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
  std::string dfa;
  regex2dfa::Regex2Dfa("^(a|b)+$", &dfa);
  fteObj.SetLanguages(dfa, 32,
                      dfa, 64);

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
  std::string plaintext_dfa;
  std::string ciphertext_dfa;
  regex2dfa::Regex2Dfa("^(a|b)+$", &plaintext_dfa);
  regex2dfa::Regex2Dfa("^.+$", &ciphertext_dfa);
  fteObj.SetLanguages(plaintext_dfa, 32,
                      ciphertext_dfa, 32);

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
