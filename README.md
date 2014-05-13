LibFTE
======

LibFTE is an encryption library that allows input plaintext and output ciphertext formats to be controlled by DFAs.

Dependencies
------------

### For building/testing

* make, m4, gcc, g++, etc.
* 32-bit emscripten (asm.js only): https://github.com/kripken/emscripten
* GMP: https://gmplib.org/
* aes: http://brgladman.org/oldsite/AES/index.php (included: thirdparty/aes)
* googletest: https://code.google.com/p/googletest/ (included: thirdparty/gtest-1.7.0)

### asm.js

[ vagrant instructions to appear here ]

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

Tested on
---------

* OSX 10.9.2 with Xcode 5.1.1, GMP 5.1.3, GCC 4.2
* Ubuntu 14.04, emscripten 1.17.0 compiled from source, build-essential

Build
-----

To build normally under gcc

```
$ cd third_party/gtest-*
$ ./configure --enable-static --disable-shared && make
$ cd ../..
$ make bin/test
...
$ ls bin
test
```

or to build under emscripten.

```
$ cd third_party/gtest-*
$ emconfigure ./configure ABI=32 --disable-assembly --disable-static --enable-shared
$ cd ../..
$ EMSCRIPTEN=1 make bin/test.js
...
$ ls bin
test.js
```

Test
----

```
$ ./bin/test (or ./bin/test.js)
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
