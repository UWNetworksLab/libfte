libfte
======

This contains initial unit tests for (un)ranking.

Dependencies
------------

* make, g++
* GMP: https://gmplib.org/
* aes: http://brgladman.org/oldsite/AES/index.php (included: thirdparty/aes)
* googletest: https://code.google.com/p/googletest/ (included: thirdparty/gtest-1.7.0)

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
[==========] Running 43 tests from 6 test cases.
[----------] Global test environment set-up.
[----------] 7 tests from CauseException
[ RUN      ] CauseException.InvalidFstFormatException1
[       OK ] CauseException.InvalidFstFormatException1 (1 ms)
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
  // setup
  fte::Key K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 128 bits, in hex
  fte::FTE fteObj = fte::FTE(VALID_DFA_5, 16, // regex="^\C{1,16}$"
                             VALID_DFA_1, 128, // regex="^(a|b){1,128}$"
                             K);
  // encrypt-then-decrypt
  std::string input_plaintext = "Hello, Word!";
  std::string ciphertext = fteObj.encrypt(X);
  std::string output_plaintext = fteObj.decrypt(Y);

  std::cout << "input_plaintext: " << X << std::endl;
  std::cout << "ciphertext: " << Y << std::endl;
  std::cout << "output_plaintext: " << Z << std::endl;
}
```

will output

```
input_plaintext: Hello, Word!
ciphertext: babbbaababababbbbabbbbaabbaabaaaaabbabaabaaabbaaaabbabaabaababaaabbbabbbaabababaaabbaabababbbbbbaabbbaaaaaaabbbbbabaabbbaaaabab
output_plaintext: Hello, Word!
```
