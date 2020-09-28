enum
{
	CRYPT_OK,
	CRYPT_NOP,
	CRYPT_INVALID_ROUNDS,
	CRYPT_INVALID_KEYSIZE,
	CRYPT_FAIL_TESTVECTOR,
};

typedef unsigned ulong32;

struct rc5_key {
   int rounds;
   ulong32 K[50];
};

typedef union Symmetric_key {
   struct rc5_key rc5;
   void *data;
} symmetric_key;

int rc5_setup(const unsigned char *key, int keylen, int num_rounds, symmetric_key *skey);
int rc5_ecb_encrypt(const unsigned char *pt, unsigned char *ct, symmetric_key *skey);
int rc5_ecb_decrypt(const unsigned char *ct, unsigned char *pt, symmetric_key *skey);
#if TESTVER
int rc5_test(void);
#endif  // TESTVER