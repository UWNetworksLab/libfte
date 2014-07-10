#include <assert.h>

#include <iostream>
#include <cstdio>
#include <ctime>

#include "regex2dfa.h"

#include "fte/fte.h"
#include "fte/encrypting/ffx/ffx.h"
#include "fte/ranking/dfa_ranker.h"

void FteExample() {
  // fte example
  std::string K("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"); // 128 bits, in hex
  fte::Fte fteObj = fte::Fte();
  fteObj.set_key(K);
  std::string regex = "^.*$";
  std::string dfa;
  regex2dfa::Regex2Dfa(regex, &dfa);
  fteObj.SetLanguages(dfa, 1024,
                      dfa, 1024);
  std::string ciphertext, output_plaintext;
  mpz_class max_plaintext;
  mpz_class base = 2;
  mpz_pow_ui(max_plaintext.get_mpz_t(), base.get_mpz_t(), 1024*8);

  gmp_randstate_t state;
  gmp_randinit_default(state);
  mpz_class input_plaintext_as_int;
  std::string input_plaintext;
  double duration;
  std::clock_t start = std::clock();
  for (uint32_t i = 0; i < 1000; ++i) {
    mpz_urandomm(input_plaintext_as_int.get_mpz_t(), state, max_plaintext.get_mpz_t());
    fte::encrypting::MpzClassToBase256(input_plaintext_as_int, 1024, &input_plaintext);
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
