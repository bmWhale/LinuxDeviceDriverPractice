#include "crypto.h"

struct crypto_skcipher *my_aes_ecb_key_setup(u8 *k, size_t k_len)
{
	char *algo = "ecb(aes)";
	struct crypto_skcipher *tfm;

	tfm = crypto_alloc_skcipher(algo, 0, CRYPTO_ALG_ASYNC);
	if (!IS_ERR(tfm))
		crypto_skcipher_setkey(tfm, k, k_len);

	return tfm;
}

int aes_ecb_encrypt(u8 *key, size_t key_sz, u8 *in, size_t in_len, u8 *out)
{
	struct crypto_skcipher *tfm;
	int err;

	tfm = my_aes_ecb_key_setup(key, key_sz);
	if (IS_ERR(tfm)) {
		pr_err("%s: could not allocate crypto aes-cbc\n", __DRIVER_NAME);
		err = -ENOMEM;
		goto my_aes_cbc_encrypt_demo_end;
	}

	err = my_aes_cbc_encrypt(tfm, in, in_len, out);

my_aes_cbc_encrypt_demo_end:
	my_aes_cbc_key_free(tfm);

	return err;
}
