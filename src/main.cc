#include <assert.h>

#include <iostream>

#include "regex2dfa.h"

#include "fte/fte.h"
#include "ffx/ffx.h"
#include "fte/ranking/dfa_ranker.h"

void FteExample() {
  // fte example
  std::string K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 128 bits, in hex
  fte::Fte fteObj = fte::Fte();
  fteObj.set_key(K);
  std::string dfa;
  regex2dfa::Regex2Dfa("^.+$", &dfa);
  fteObj.SetLanguages(dfa, 128,
                      dfa, 128);
  std::string input_plaintext = "Hello, Word!";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  std::cout << "fte:" << std::endl;
  std::cout << "- input_plaintext: " << input_plaintext << std::endl;
  std::cout << "- ciphertext: " << ciphertext << std::endl;
  std::cout << "- output_plaintext: " << output_plaintext << std::endl;

  assert(input_plaintext == output_plaintext);
}

void FfxExample() {
  // ffx example
  uint32_t radix = 2;
  ffx::Ffx ffxObj = ffx::Ffx(radix);
  std::string K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; //    128 bits, in hex
  ffxObj.SetKey(K);
  mpz_class input_plaintext = 65535;
  uint32_t input_plaintext_len = 16; // in bits
  mpz_class ciphertext, output_plaintext;
  ffxObj.Encrypt(input_plaintext, input_plaintext_len, &ciphertext);
  ffxObj.Decrypt(ciphertext, input_plaintext_len, &output_plaintext);

  std::cout << "ffx:" << std::endl;
  std::cout << "- input_plaintext: " << input_plaintext.get_str() << std::endl;
  std::cout << "- ciphertext: " << ciphertext.get_str() << std::endl;
  std::cout << "- output_plaintext: " << output_plaintext.get_str() << std::endl;

  assert(input_plaintext == output_plaintext);
}

void RankingExample() {

  // ranking example
  uint32_t N  = 8;
  fte::ranking::DfaRanker rankerObj = fte::ranking::DfaRanker();
  std::string dfa;
  regex2dfa::Regex2Dfa("^(a|b)+$", &dfa);
  rankerObj.SetLanguage(dfa, N);
  std::string input_string = "bbbbbbbb";
  mpz_class output_rank;
  std::string output_string;
  rankerObj.Rank(input_string, &output_rank);
  rankerObj.Unrank(output_rank, &output_string);

  std::cout << "ranking:" << std::endl;
  std::cout << "- input_string: " << input_string << std::endl;
  std::cout << "- rank: " << output_rank.get_str() << std::endl;
  std::cout << "- output_string: " << output_string << std::endl;

  assert(input_string == output_string);

}

int main(int argc, char **argv) {
  FteExample();
  FfxExample();
  RankingExample();
  return 0;
}
