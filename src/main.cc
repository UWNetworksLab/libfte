#include <iostream>

#include "fte/ranker.h"
#include "tests/dfas.h"

int main(int argc, char **argv) {
    uint32_t N  = 8;
    fte::ranker rankerObj(VALID_DFA_1, N);
    std::string X = "bbbbbbbb";
    mpz_class C   = rankerObj.rank(X);
    std::string Y = rankerObj.unrank(C);
    std::cout << "X: " << X << std::endl;
    std::cout << "C: " << C << std::endl;
    std::cout << "Y: " << Y << std::endl;
    return 0;
}
