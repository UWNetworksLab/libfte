#include <iostream>

#include "ranker.h"
#include "tests/dfas.h"

int main(int argc, char **argv) {
  ranker rankerObj(VALID_DFA_1, 16);
  std::string X = rankerObj.unrank(0);
  mpz_class C   = rankerObj.rank(X);
  std::cout << "X: " << X << std::endl;
  std::cout << "C: " << C << std::endl;
  return 0;
}
