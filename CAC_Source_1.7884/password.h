void memzero_s(char *p, size_t size);
void calc_simple_xor(unsigned char *password, size_t size);

class DtorZero
{
public:
	DtorZero(char *tData, size_t tSize) : data(tData), size(tSize) { }
	~DtorZero() { memzero_s(data, size); }

private:
	char *const data;
	const size_t size;
};
