#ifdef __AES_ECB_H
#define __AES_ECB_H
struct crypto_skcipher *aes_ecb_key_setup(u8 *k, size_t k_len);
int aes_ecb_encrypt(u8 *key, size_t key_sz, u8 *in, size_t in_len, u8 *out);
#endif
