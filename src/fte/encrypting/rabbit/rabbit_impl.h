#ifndef __RABBIT_H__
#define __RABBIT_H__

#include <stdint.h>

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; KEYSIZE(i) <= MAXKEYSIZE; ++i)
 *   {
 *     keysize = KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bytes.
 */

#define MAXKEYSIZE 128
#define KEYSIZE(i) (128 + (i)*32)

#define MAXIVSIZE 64
#define IVSIZE(i) (64 + (i)*64)

const uint32_t BLOCKLENGTH = 16;


/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

struct RABBIT_ctx {
  uint32_t x[8];
  uint32_t c[8];
  uint32_t carry;
};

struct ctx {
  /*
   * Put here all state variable needed during the encryption process.
   */
  RABBIT_ctx master_ctx;
  RABBIT_ctx work_ctx;
};

class RabbitCryptor {
 public:
  RABBIT_ctx _master_ctx;
  RABBIT_ctx _work_ctx;

  static const uint32_t kDefaultKeySize = KEYSIZE(0) / 8;
  static const uint32_t kDefaultIvSize = IVSIZE(0) / 8;

  /*
   * Encryption/decryption of arbitrary length messages.
   *
   * For efficiency reasons, the API provides two types of
   * encrypt/decrypt functions. The encrypt_bytes() function
   * (declared here) encrypts byte strings of arbitrary length, while
   * the encrypt_blocks() function (defined later) only accepts
   * lengths which are multiples of BLOCKLENGTH.
   *
   * The user is allowed to make multiple calls to
   * encrypt_blocks() to incrementally encrypt a long message,
   * but he is NOT allowed to make additional encryption calls once he
   * has called encrypt_bytes() (unless he starts a new message
   * of course). For example, this sequence of calls is acceptable:
   *
   * keysetup();
   *
   * ivsetup();
   * encrypt_blocks();
   * encrypt_blocks();
   * encrypt_bytes();
   *
   * ivsetup();
   * encrypt_blocks();
   * encrypt_blocks();
   *
   * ivsetup();
   * encrypt_bytes();
   *
   * The following sequence is not:
   *
   * keysetup();
   * ivsetup();
   * encrypt_blocks();
   * encrypt_bytes();
   * encrypt_blocks();
   */

  /*
   * Key setup. It is the user's responsibility to select the values of
   * keysize and ivsize from the set of supported values specified
   * above.
   */
  void keysetup(const uint8_t *key, uint32_t keysize, uint32_t ivsize);


  /*
   * IV setup. After having called keysetup(), the user is
   * allowed to call ivsetup() different times in order to
   * encrypt/decrypt different messages with the same key but different
   * IV's.
   */
  void ivsetup(const uint8_t *iv);

  void encrypt_bytes(const uint8_t *plaintext, uint8_t *ciphertext, uint32_t msglen) {
    process_bytes(0, plaintext, ciphertext, msglen);
  }

  void decrypt_bytes(const uint8_t *ciphertext, uint8_t *plaintext, uint32_t msglen) {
    process_bytes(1, ciphertext, plaintext, msglen);
  }

  void process_bytes(int action,  /* 0 = encrypt; 1 = decrypt; */
                     const uint8_t *input,
                     uint8_t *output,
                     uint32_t msglen);

  /*
   * For testing purposes it can sometimes be useful to have a function
   * which immediately generates keystream without having to provide it
   * with a zero plaintext. If your cipher cannot provide this function
   * (e.g., because it is not strictly a synchronous cipher), please
   * reset the GENERATES_KEYSTREAM flag.
   */
  void keystream_bytes(
    uint8_t *keystream,
    uint32_t length);                /* Length of keystream in bytes. */



  void encrypt_blocks(const uint8_t *plaintext, uint8_t *ciphertext, uint32_t blocks) {
    process_blocks(0, plaintext, ciphertext, blocks);
  }

  void decrypt_blocks(const uint8_t *ciphertext, uint8_t *plaintext, uint32_t blocks) {
    process_blocks(1, ciphertext, plaintext, blocks);
  }

  void process_blocks(
    int action,                 /* 0 = encrypt; 1 = decrypt; */
    const uint8_t *input,
    uint8_t *output,
    uint32_t blocks);                /* Message length in blocks. */

  void keystream_blocks(
    uint8_t *keystream,
    uint32_t blocks);                /* Keystream length in blocks. */

};

#endif  // __RABBIT_H__
