#include <assert.h>

#include "aes/aes.h"

#include "ffx/conversions.h"
#include "ffx/encryption.h"

namespace ffx {

mpz_class aes_ecb ( const Key K, const mpz_class X, const uint32_t X_len ) {
  mpz_class retval = 0;

  assert ( ( X_len % kFFXKeyLengthInBits ) == 0 );

  uint32_t byte_string_len = X_len / 8;

  aes_encrypt_ctx * ctx = new aes_encrypt_ctx[1];
  unsigned char * key = new unsigned char[kFFXKeyLengthInBytes];
  unsigned char * inBuffer = new unsigned char[byte_string_len];
  unsigned char * outBuffer = new unsigned char[byte_string_len];


  uint8_t i = 0;
  for ( i = 0; i < byte_string_len; i++ ) {
    inBuffer[i] = 0;
    outBuffer[i] = 0;
  }

  for ( i = 0; i < kFFXKeyLengthInBytes; ++i ) {
    key[i] = 0x00;
  }

  mpz_to_char_array ( X, byte_string_len, inBuffer );
  fte_key_to_char_array ( K.getKey(), kFFXKeyLengthInBytes, key );

  aes_init();
  aes_encrypt_key128 ( key, ctx );
  aes_ecb_encrypt ( inBuffer, outBuffer, byte_string_len, ctx );

  char_array_to_mpz ( outBuffer, byte_string_len, retval );

  // cleanup
  delete[] ctx;
  delete[] key;
  delete[] inBuffer;
  delete[] outBuffer;

  ctx = NULL;
  key = NULL;
  inBuffer = NULL;
  outBuffer = NULL;

  return retval;
}

mpz_class aes_cbc_mac ( const Key K, const mpz_class X, const uint32_t X_len ) {
  mpz_class retval = 0;

  assert ( ( X_len % kFFXKeyLengthInBits ) == 0 );

  uint32_t byte_string_len = X_len / 8;

  aes_encrypt_ctx * ctx = new aes_encrypt_ctx[1];
  unsigned char * iv = new unsigned char[kFFXKeyLengthInBytes];
  unsigned char * key = new unsigned char[kFFXKeyLengthInBytes];
  unsigned char * inBuffer = new unsigned char[byte_string_len];
  unsigned char * outBuffer = new unsigned char[byte_string_len];

  uint8_t i = 0;
  for ( i = 0; i < byte_string_len; i++ ) {
    inBuffer[i] = 0;
    outBuffer[i] = 0;
  }

  for ( i = 0; i < kFFXKeyLengthInBytes; ++i ) {
    iv[i] = 0x00;
    key[i] = 0x00;
  }

  mpz_to_char_array ( X, byte_string_len, inBuffer );
  fte_key_to_char_array ( K.getKey(), kFFXKeyLengthInBytes, key );

  aes_init();
  aes_encrypt_key128 ( key, ctx );
  aes_cbc_encrypt ( inBuffer, outBuffer, byte_string_len, iv, ctx );

  char_array_to_mpz ( outBuffer, byte_string_len, retval );

  retval = extract_bit_range ( retval,
                               byte_string_len * 8,
                               byte_string_len * 8 - 128,
                               byte_string_len * 8 - 1 );

  // cleanup
  delete[] ctx;
  delete[] iv;
  delete[] key;
  delete[] inBuffer;
  delete[] outBuffer;

  ctx = NULL;
  iv = NULL;
  key = NULL;
  inBuffer = NULL;
  outBuffer = NULL;

  return retval;
}

} // namespace ffx
