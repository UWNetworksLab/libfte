#include <assert.h>

#include <iostream>
#include <cstdio>
#include <ctime>

#include "fte/fte.h"
#include "ffx/ffx.h"
#include "fte/ranking/dfa_ranker.h"
#include "fte/ranking/sample_dfas.h"

void FteExample() {
  // fte example
  std::string K("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"); // 128 bits, in hex
  fte::Fte fteObj = fte::Fte();
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_5, 1024,
                      VALID_DFA_5, 1024);
  std::string input_plaintext = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string ciphertext, output_plaintext;

  double duration;
  std::clock_t start = std::clock();
  for (uint32_t i = 0; i < 1000; ++i) {
    ciphertext = "";
    fteObj.Encrypt(input_plaintext, &ciphertext);
  }
  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  std::cout << "Avg. cost (ms) of ('^.+$',1024) -> ('^.+$',1024): " << duration << std::endl;

  fteObj.Decrypt(ciphertext, &output_plaintext);
  assert(input_plaintext == output_plaintext);
}

int main(int argc, char **argv) {
  FteExample();
  return 0;
}
