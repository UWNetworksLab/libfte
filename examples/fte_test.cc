#include <stdio.h>
#include "gmpxx.h"

int main() {
  mpz_class X = mpz_class("9999999999999999999999999999999999999");
  X("%s\n",X.get_str());
}
