#include <assert.h>

#include <iostream>
#include <cstdio>
#include <ctime>

#include "fte/fte.h"
#include "ffx/ffx.h"
#include "fte/ranking/dfa_ranker.h"

void FfxExample() {
  // ffx example
  uint32_t radix = 2;
  std::string K("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"); //    128 bits, in hex
  ffx::Ffx ffxObj = ffx::Ffx(radix);
  ffxObj.SetKey(K);
  mpz_class input_plaintext = 1;
  uint32_t input_plaintext_len = 1024 * 8; // in bits
  mpz_class ciphertext, output_plaintext;

  double duration;
  std::clock_t start = std::clock();
  for (uint32_t i = 0; i < 1000; ++i) {
    ffxObj.Encrypt(input_plaintext, input_plaintext_len, &ciphertext);
  }
  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  std::cout << "Avg. cost (ms) of FFX[2] encrypt of a 1024-byte string: " << duration << std::endl;

  ffxObj.Decrypt(ciphertext, input_plaintext_len, &output_plaintext);
  assert(input_plaintext == output_plaintext);
}

int main(int argc, char **argv) {
  FfxExample();
  return 0;
}
