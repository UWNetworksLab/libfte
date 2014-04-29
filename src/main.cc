#include <iostream>

#include "ranker.h"
#include "tests/dfas.h"

int main(int argc, char **argv) {
    ranker rankerObj(VALID_DFA_1, 8);
    std::string X = "bbbbbbbb";
    mpz_class C   = rankerObj.rank(X);
    std::string Y = rankerObj.unrank(C);
    std::cout << "X: " << X << std::endl;
    std::cout << "C: " << C << std::endl;
    std::cout << "Y: " << Y << std::endl;
    return 0;
}
