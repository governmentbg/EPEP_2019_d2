#include "aes.h"
#include "myrand.h"

class sn_cls
{
public:
	static void init() { AesGenTables(); }

	char sn[27 + 1];
	int court, users, flags;
	int day, month, z_year;

	struct VK
	{
		int version;
		Byte key[16];
	};

protected:
	UInt32 aes[AES_NUM_IVMRK_WORDS];
	Byte input[16];
	Byte block[16];
	unsigned *seed;

	double get_full();
	void seed_key(const Byte *key, int version) { seed = (unsigned *) (key + 2 + version % (12 - (version >= 6275))); }

#ifndef COURT_TYPE
	enum { SN_MAXKEYS = 5 };
	static VK vk[SN_MAXKEYS];

	const Byte *find_key(int version);
#endif  // COURT_TYPE
};

#ifndef COURT_TYPE
class sn_enc : public sn_cls
{
public:
	void make_key(int version);
	void encode();
};
#endif  // COURT_TYPE

class sn_dec : public sn_cls
{
public:
	enum
	{
		SNE_SYNTAX	= 1,
		SNE_BVALUE	= 2,
		SNE_CVALUE	= 3,
		SNE_COURT	= 4,
		SNE_USERS	= 5,
		SNE_VDATE	= 6,	// CAC only
		SNE_CTYPE	= 7	// CAC only
	};

	void make_key(const Byte *key, int version);
#ifndef COURT_TYPE
	void make_key(int version) { make_key(find_key(version), version); }
	void verify(sn_enc &that, int version);
#endif
	bool precode();
	int decode();
};
