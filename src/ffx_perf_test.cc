#include <assert.h>

#include <iostream>

#include "fte/fte.h"
#include "ffx/ffx.h"
#include "fte/ranking/dfa_ranker.h"
#include "fte/ranking/sample_dfas.h"

void FfxExample() {
  // ffx example
  uint32_t radix = 2;
  std::string K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; //    128 bits, in hex
  ffx::Ffx ffxObj = ffx::Ffx(radix);
  ffxObj.SetKey(K);
  mpz_class input_plaintext = 1;
  uint32_t input_plaintext_len = 1024 * 8; // in bits
  mpz_class ciphertext, output_plaintext;

  for (uint32_t i = 0; i <1000; ++i) {
    ffxObj.Encrypt(input_plaintext, input_plaintext_len, &ciphertext);
    ffxObj.Decrypt(ciphertext, input_plaintext_len, &output_plaintext);
  }

  assert(input_plaintext == output_plaintext);
}

int main(int argc, char **argv) {
  FfxExample();
  return 0;
}
