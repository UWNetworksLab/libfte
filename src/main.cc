#include <assert.h>
#include <iostream>

// fte includes
#include "fte/fte.h"
#include "tests/dfas.h"

// ffx includes
#include "ffx/ffx.h"

// ranking includes
#include "fte/ranking/dfa.h"

void fte_example() {
  // fte example
  fte::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 128 bits, in hex
  fte::FTE fteObj = fte::FTE ( VALID_DFA_5, 16,
                               VALID_DFA_1, 128,
                               K );
  std::string X = "Hello, Word!";
  std::string Y = fteObj.encrypt ( X );
  std::string Z = fteObj.decrypt ( Y );

  std::cout << "fte:" << std::endl;
  std::cout << "- X: " << X << std::endl;
  std::cout << "- Y: " << Y << std::endl;
  std::cout << "- Z: " << Z << std::endl;

  assert ( X == Z );
}

void ffx_example() {
  // ffx example
  uint32_t radix = 2;
  ffx::FFX ffxObj = ffx::FFX ( radix );
  ffx::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 128 bits, in hex
  mpz_class X = 65535;
  uint32_t X_len = 16; // in bits
  mpz_class Y = ffxObj.encrypt ( K, X, X_len );
  mpz_class Z = ffxObj.decrypt ( K, Y, X_len );

  std::cout << "ffx:" << std::endl;
  std::cout << "- X: " << X << std::endl;
  std::cout << "- Y: " << Y << std::endl;
  std::cout << "- Z: " << Z << std::endl;

  assert ( X == Z );
}

void ranking_example() {

  // ranking example
  uint32_t N  = 8;
  fte::ranking::dfa rankerObj ( VALID_DFA_1, N );
  std::string X = "bbbbbbbb";
  mpz_class Y   = rankerObj.rank ( X );
  std::string Z = rankerObj.unrank ( Y );

  std::cout << "ranking:" << std::endl;
  std::cout << "- X: " << X << std::endl;
  std::cout << "- Y: " << Y << std::endl;
  std::cout << "- Z: " << Z << std::endl;

  assert ( X == Z );

}

int main ( int argc, char **argv ) {
  fte_example();
  ffx_example();
  ranking_example();
  return 0;
}
