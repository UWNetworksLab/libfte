libfte
======

This contains initial unit tests for (un)ranking.

Dependencies
------------

* make, g++
* GMP: https://gmplib.org/
* googletest: https://code.google.com/p/googletest/ (included)

### OSX

On OSX: install Xcode, homebrew, then

```
brew install --build-from-source gmp
```

### Debian/Ubuntu

On Debian/Ubuntu

```
apt-get install build-essential
apt-get install libgmp-dev
```

### Windows

TODO

Tested on
---------

* OSX 10.9.2 with Xcode 5.1.1 and GMP 5.1.3

Build
-----

```
$ make
g++ -g -Wall -Isrc -Ithirdparty/gtest-1.7.0/include -c -o src/test.o src/test.cc
g++ -g -Wall -Isrc -Ithirdparty/gtest-1.7.0/include -c -o src/ranker.o src/ranker.cc
...
$ ls bin
main	test
```

Test
----

```
$ make test
[==========] Running 6 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 6 tests from CauseException
[ RUN      ] CauseException.InvalidFstFormatException1
[       OK ] CauseException.InvalidInputNoAcceptingPathsException1 (0 ms)
...
[ RUN      ] CauseException.InvalidRankInputException
[       OK ] CauseException.InvalidRankInputException (0 ms)
[----------] 6 tests from CauseException (1 ms total)

[----------] Global test environment tear-down
[==========] 6 tests from 1 test case ran. (1 ms total)
[  PASSED  ] 6 tests.
```

fte-encryption examples
-----------------------

```
#include "fte.h"

int main(int argc, char **argv) {
    fte fteObj(..., ..., ...);
    fte.encrypt("hello");
    fteObj.decrypt(ciphertext);
    return 0;
}

```

ffx-encryption example
----------------------

TODO

ranking example
---------------

```c++
#include <iostream>

#include "ranker.h"
#include "tests/dfas.h"

int main(int argc, char **argv) {
    uint32_t N = 8;
    ranker rankerObj(VALID_DFA_1, N);
    std::string X = "bbbbbbbb";
    mpz_class C   = rankerObj.rank(X);
    std::string Y = rankerObj.unrank(C);
    std::cout << "X: " << X << std::endl;
    std::cout << "C: " << C << std::endl;
    std::cout << "Y: " << Y << std::endl;
    return 0;
}
```

which outputs

```
X: bbbbbbbb
C: 509
Y: bbbbbbbb
```

because ```C = 2^1 + 2^2 + ... + 2^7 + (2^N - 1)```.
