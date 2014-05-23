#include <assert.h>

#include <iostream>

#include "fte/fte.h"
#include "ffx/ffx.h"
#include "fte/ranking/dfa.h"

void FteExample() {
  // fte example
  fte::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 128 bits, in hex
  fte::FTE fteObj = fte::FTE(VALID_DFA_5, 16,
                             VALID_DFA_1, 128,
                             K);
  std::string input_plaintext = "Hello, Word!";
  std::string ciphertext = fteObj.Encrypt(input_plaintext);
  std::string output_plaintext = fteObj.Decrypt(ciphertext);

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
  ffx::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; //    128 bits, in hex
  mpz_class input_plaintext = 65535;
  uint32_t input_plaintext_len = 16; // in bits
  mpz_class ciphertext = ffxObj.Encrypt(K, input_plaintext, input_plaintext_len);
  mpz_class output_plaintext = ffxObj.Decrypt(K, ciphertext, input_plaintext_len);

  std::cout << "ffx:" << std::endl;
  std::cout << "- input_plaintext: " << input_plaintext.get_str() << std::endl;
  std::cout << "- ciphertext: " << ciphertext.get_str() << std::endl;
  std::cout << "- output_plaintext: " << output_plaintext.get_str() << std::endl;

  assert(input_plaintext == output_plaintext);
}

void RankingExample() {

  // ranking example
  uint32_t N  = 8;
  fte::ranking::DfaRanker rankerObj(VALID_DFA_1, N);
  std::string input_plaintext = "bbbbbbbb";
  mpz_class ciphertext   = rankerObj.Rank(input_plaintext);
  std::string output_plaintext = rankerObj.Unrank(ciphertext);

  std::cout << "ranking:" << std::endl;
  std::cout << "- input_plaintext: " << input_plaintext << std::endl;
  std::cout << "- ciphertext: " << ciphertext.get_str() << std::endl;
  std::cout << "- output_plaintext: " << output_plaintext << std::endl;

  assert(input_plaintext == output_plaintext);

}

int main(int argc, char **argv) {
  FteExample();
  FfxExample();
  RankingExample();
  return 0;
}
