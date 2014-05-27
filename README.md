LibFTE
======

### Overview

LibFTE is an encryption library that allows input plaintext and output ciphertext formats to be defined by regular expressions.
Specifically one can instantiate an FTE scheme FTE[R_in,R_out], where R_in specifies a regular language for the input plaintexts and R_out describes a regular language for the ouput ciphertexts.
Then, it's the job of the FTE scheme to encrypt plaintexts in the language L(R_in) into ciphertexts in the language of L(R_out).

Naturally there are limitations on what regular expressions may be used to instantiate an FTE scheme.
As one example we can't have |L(R_in)| > |L(R_out)|, otherwise encryption is not injective.
Checks are performed within LibFTE to ensure impossible schemes result in an error.

See "LibFTE details" below, for more information.

Quickstart
----------

### Platforms

* OSX 10.9.2, 64-bit, Xcode 5.1.1, Apple LLVM version 5.1
* Ubuntu 14.04, 32-bit, emscripten 1.18.0 compiled from source, build-essential

### Building

Ensure you have the following installed

* Standard build tools: make, m4, gcc, g++, etc.
* GMP: https://gmplib.org/

then

```shell
$ ./configure
$ make
```

Running ```make``` will produce ```.libs/libfte.a``` and ```bin/test```.

### Testing

```shell
$ make test
[==========] Running 64 tests from 9 test cases.
[----------] Global test environment set-up.
[----------] 2 tests from SanityCheckMatch
[ RUN      ] SanityCheckMatch.Test1
[       OK ] SanityCheckMatch.Test1 (0 ms)
...
[ RUN      ] ErrorTest.NoLanguageSet
[       OK ] ErrorTest.NoLanguageSet (0 ms)
[----------] 8 tests from ErrorTest (1 ms total)

[----------] Global test environment tear-down
[==========] 64 tests from 9 test cases ran. (1884 ms total)
[  PASSED  ] 64 tests.
```

FTE encryption example
----------------------

```c++
#include "fte/fte.h"

void main() {
  std::string K = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"; // 128 bits, in hex
  fte::Fte fteObj = fte::Fte();
  fteObj.set_key(K);
  fteObj.SetLanguages(VALID_DFA_5, 16,   // ^\C{,16}$
                      VALID_DFA_1, 128); // ^(a|b){,129}$
  std::string input_plaintext = "Hello, Word!";
  std::string ciphertext, output_plaintext;
  fteObj.Encrypt(input_plaintext, &ciphertext);
  fteObj.Decrypt(ciphertext, &output_plaintext);

  std::cout << "input_plaintext: " << input_plaintext << std::endl;
  std::cout << "ciphertext: " << ciphertext << std::endl;
  std::cout << "output_plaintext: " << output_plaintext << std::endl;
}
```

will output

```shell
input_plaintext: Hello, Word!
ciphertext: babbbaababababbbbabbbbaabbaabaaaaabbabaabaaabbaaaabbabaabaababaaabbbabbbaabababaaabbaabababbbbbbaabbbaaaaaaabbbbbabaabbbaaaabab
output_plaintext: Hello, Word!
```

LibFTE details
--------------

The LibFTE library is based on the rank-encipher-unrank constuction presented in [FPE1] and revisited in [FTE2].
Let FTE[R_in, R_out] be an FTE scheme where R_in is a regular expression describing the set of possible plaintexts, and R_out is a regular expression describing the set of possible ciphertexts.
Also, let the function rank_R be a bijective mapping between the elements of language L(R) and the integers {0,1,...,|L(R)|-1}.
Then, the construction is realized as follows:

1. On input of a plaintext M in L(R_in), calculate a = rank(X).
2. Using some encryption scheme, encrypt a to ciphertext b.
3. Interpret b as an integer, if b is in {0,1,...,|L(R_out)|-1}, then goto 4, else go back to step 2 and reencrypt b.
4. Calculate unrank(b) as our output ciphertext.

We may visualize this as follows:

![LibFTE rank-encipher-urank construction](images/fte-workflow.png "The rank-encipher-unrank approach to Format-Transforming Encryption.")

In our diagram the "valid?" check is the process of determining if b can be unranked into language L(R_out).
For DFA-based ranking this is an inequality.
However, in general, it turns out that the validation is not always so simple, hence the generalization.

### Our implementation

* We use FFX [FFX1, FFX2] as our encryption scheme in step 2. This is used a variable-input length blockcipher to encrypt an n-bit string into another n-bit string.
* We current perform ranking using the DFA representation of a language, as described in [FTE1].
* We implemnt cycle walking for the step 2/3 loop in our FTE scheme. This is the strategy of iteratively applying encrypt to N (i.e. encrypt(...encrypt(N))) until it is a ciphertext that can be unranked. If one is using a randomized scheme they may use rejction sampling.

### Challenges

* We require that |L(R_out)| > |L(R_in)|.
* We must choose an encryption scheme in step 2 that maximizes the probability that we produce a ciphertext that can be unranked into R_out.
* The encryption scheme used in step 2 should have minimal (or no) ciphertext expansion.
* The (un)ranking aglorithms presented in [GS] are too slow. When implemented DFA-based ranking, one should use the algorithmic improvements presented in [FTE1, FTE2].

### Other features

* In [FTE2] we also explore NFA-based ranking, deterministic and randomized encryption, and FFX for radicies other than 2. These features, and more, will appear in a future version of LibFTE.

References and Acknowledgements
-------------------------------

LibFTE is based on concepts and algorithms from the following papers.

* [FTE1] [Protocol Misidentification Made Easy with Format-Transforming Encryption](http://eprint.iacr.org/2012/494.pdf)
Kevin P. Dyer, Scott E. Coull, Thomas Ristenpart and Thomas Shrimpton.
In proceedings of the ACM Conference on Computer and Communications Security (CCS), 2013. 
* [FTE2] LibFTE: A User-Friendly Toolkit for Constructing Practical Format-Abiding Encryption Schemes
Daniel Luchaup, Kevin P. Dyer, Somesh Jha, Thomas Ristenpart and Thomas Shrimpton.
To appear in the proceedings of USENIX Security 2014
* [FPE1] [Format Preserving Encryption](http://eprint.iacr.org/2009/251.pdf)
Mihir Bellare, Tom Ristenpart, Phillip Rogaway, and Till Stegers. SAC 2009. LNCS 5867, Springer, pp. 295-312, 2009.
* [FFX1] [The FFX Mode of Operation for Format Preserving Encryption](http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec.pdf)
Mihir Bellare, Phillip Rogaway, and Terence Spies. Unpublished manuscript, submitted to NIST for possible standardization. February 20, 2010.
* [FFX2] [Addendum to “The FFX Mode of Operation for Format Preserving Encryption”](http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ffx/ffx-spec2.pdf)
Mihir Bellare, Phillip Rogaway, and Terence Spies. Unpublished manuscripts, submitted to NIST for possible standardization. September 3, 2010.
* [GS] A. Goldberg and M. Sipser. Compression and ranking. In Proceedings of the 17th Annual ACM symposium on Theory of computing, STOC ’85, 1985

The LibFTE implementation depends upon the following libraries:

* GMP: http://gmplib.org/
* aes: http://brgladman.org/oldsite/AES/index.php
* googletest: https://code.google.com/p/googletest

