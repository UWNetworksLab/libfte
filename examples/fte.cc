#include <stdio.h>

#include <gmpxx.h>

int main() {
  mpz_class X = mpz_class("9999999999999999999999999999999999999");
  X *= 2;
  X  = X << 10;
  X  = X ^ 1;
  printf("%s\n", X.get_str().c_str());
}
