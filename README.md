LibFTE
======

LibFTE is an encryption library that allows input plaintext and output ciphertext formats to be controlled by DFAs.

Dependencies
------------

### For building/testing

* make, m4, gcc, g++, etc.

### Third Party Libraries (Included)

* GMP: https://gmplib.org/ (included: third\_party/gmp-6.0.0)
* aes: http://brgladman.org/oldsite/AES/index.php (included: third\_party/aes)
* googletest: https://code.google.com/p/googletest/ (included: third\_party/gtest-1.7.0)

Tested on
---------

* OSX 10.9.2, 64-bit, Xcode 5.1.1, Apple LLVM version 5.1
* Ubuntu 14.04, 32-bit, emscripten 1.18.0 compiled from source, build-essential

Build
-----

```
$ ./configure
$ make [bin/test|bin/test.js]
```

Running ```make``` will produce ```.libs/libfte.a``` and ```bin/test``` or ```bin/test.js```.

There is currently no ```make install``` target.

Test
----

```
$ ./bin/test (or nodejs ./bin/test.js)
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
  std::string ciphertext = fteObj.encrypt(input_plaintext);
  std::string output_plaintext = fteObj.decrypt(ciphertext);

  std::cout << "input_plaintext: " << input_plaintext << std::endl;
  std::cout << "ciphertext: " << ciphertext << std::endl;
  std::cout << "output_plaintext: " << output_plaintext << std::endl;
}
```

will output

```
input_plaintext: Hello, Word!
ciphertext: babbbaababababbbbabbbbaabbaabaaaaabbabaabaaabbaaaabbabaabaababaaabbbabbbaabababaaabbaabababbbbbbaabbbaaaaaaabbbbbabaabbbaaaabab
output_plaintext: Hello, Word!
```
