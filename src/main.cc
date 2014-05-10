#include <assert.h>
#include <iostream>

// fte includes
#include "fte/fte.h"
#include "tests/dfas.h"

// ffx includes
#include "ffx/ffx.h"

// ranking includes
#include "fte/ranking/dfa.h"

int main(int argc, char **argv) {
  // fte example
  fte::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 128 bits, in hex
  fte::FTE fteObj = fte::FTE(VALID_DFA_5, 16,
                             VALID_DFA_1, 128,
                             K);
  std::string X = "Hello, Word!";
  std::string Y = fteObj.encrypt(X);
  std::string Z = fteObj.decrypt(Y);

  std::cout << "fte:" << std::endl;
  std::cout << "- X: " << X << std::endl;
  std::cout << "- Y: " << Y << std::endl;
  std::cout << "- Z: " << Z << std::endl;

  assert(X == Z);
}
