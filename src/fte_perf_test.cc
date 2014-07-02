#include <assert.h>

#include <iostream>
#include <cstdio>
#include <ctime>

#include "regex2dfa.h"

#include "fte/fte.h"
#include "ffx/ffx.h"
#include "fte/ranking/dfa_ranker.h"

void FteExample() {
  // fte example
  std::string K("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"); // 128 bits, in hex
  fte::Fte fteObj = fte::Fte();
  fteObj.set_key(K);
  std::string regex = "^.+$";
  std::string dfa;
  regex2dfa::Regex2Dfa(regex, &dfa);
  fteObj.SetLanguages(dfa, 1024,
                      dfa, 1024);
  std::string input_plaintext = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string ciphertext, output_plaintext;

  double duration;
  std::clock_t start = std::clock();
  for (uint32_t i = 0; i < 1024; ++i) {
    fteObj.Encrypt(input_plaintext, &ciphertext);
  }
  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  std::cout << "Avg. cost (ms) of ('" << regex << "',1024) -> ('" << regex << "',1024): " << duration << std::endl;

  fteObj.Decrypt(ciphertext, &output_plaintext);
  assert(input_plaintext == output_plaintext);
}

int main(int argc, char **argv) {
  FteExample();
  return 0;
}
