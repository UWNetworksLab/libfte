LibFTE
======

LibFTE is an encryption library that allows input plaintext and output ciphertext formats to be controlled by DFAs.

Under the hood, LibFTE implements the rank-encipher-unrank approach to Format-Transforming Encryption.

![LibFTE rank-encipher-urank construction](images/fte-workflow.pdf "The rank-encipher-unrank approach to Format-Transforming Encryption.")

[explain rTe]


Dependencies
------------

### For building/testing

* make, m4, gcc, g++, etc.
* GMP: https://gmplib.org/

Tested on
---------

* OSX 10.9.2, 64-bit, Xcode 5.1.1, Apple LLVM version 5.1
* Ubuntu 14.04, 32-bit, emscripten 1.18.0 compiled from source, build-essential

Build
-----

```
$ ./configure
$ make
```

Running ```make``` will produce ```.libs/libfte.a``` and ```bin/test```.

Test
----

```
$ ./bin/test
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


References and Acknowledgements
-------------------------------

LibFTE is based on concepts and algorithms from the following papers.

* [Protocol Misidentification Made Easy with Format-Transforming Encryption](http://eprint.iacr.org/2012/494.pdf)
Kevin P. Dyer, Scott E. Coull, Thomas Ristenpart and Thomas Shrimpton.
In proceedings of the ACM Conference on Computer and Communications Security (CCS), 2013. 
* [LibFTE: A User-Friendly Toolkit for Constructing Practical Format-Abiding Encryption Schemes](https://kpdyer.com/publications/usenix2014-libfte-preprint.pdf)
Daniel Luchaup, Kevin P. Dyer, Somesh Jha, Thomas Ristenpart and Thomas Shrimpton.
To appear in the proceedings of USENIX Security 2014
* [The FFX Mode of Operation for Format Preserving Encryption](http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec.pdf)
Mihir Bellare, Phillip Rogaway, and Terence Spies. Unpublished manuscript, submitted to NIST for possible standardization. February 20, 2010.
* [Addendum to “The FFX Mode of Operation for Format Preserving Encryption”](http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec2.pdf)
Mihir Bellare, Phillip Rogaway, and Terence Spies. Unpublished manuscripts, submitted to NIST for possible standardization. September 3, 2010.

LibFTE depends upon the following libraries:

* GMP: http://gmplib.org/
* aes: http://brgladman.org/oldsite/AES/index.php
* googletest: https://code.google.com/p/googletest

