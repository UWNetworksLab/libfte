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
[ RUN      ] RankerNormalUsage.Test10
[       OK ] RankerNormalUsage.Test10 (1 ms)
[----------] 10 tests from RankerNormalUsage (6 ms total)

[----------] Global test environment tear-down
[==========] 43 tests from 6 test cases ran. (20 ms total)
[  PASSED  ] 43 tests.
```

FTE encryption example
----------------------

```c++
#include "fte/fte.h"

int main() {
  fte::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 128 bits, in hex
  fte::FTE fteObj = fte::FTE(VALID_DFA_5, 16, // regex="^\C+$"
                             VALID_DFA_1, 128, // regex="^(a|b)+$"
                             K);
  std::string X = "Hello, Word!";
  std::string Y = fteObj.encrypt(X);
  std::string Z = fteObj.decrypt(Y);

  std::cout << "fte:" << std::endl;
  std::cout << "- X: " << X << std::endl;
  std::cout << "- Y: " << Y << std::endl;
  std::cout << "- Z: " << Z << std::endl;
}
```

will output

```
fte:
- X: Hello, Word!
- Y: babbbaababababbbbabbbbaabbaabaaaaabbabaabaaabbaaaabbabaabaababaaabbbabbbaabababaaabbaabababbbbbbaabbbaaaaaaabbbbbabaabbbaaaabab
- Z: Hello, Word!
```