const int
	BASE_PERIOD	= 65,
	MIN_DELTA	= 18,
	BASE_COUNT	= 33728,
	CALC_COUNT	= 64,
	RAND_RANGE	= 1008;

const long LONG_START	= -681879291; //-101902203;

struct TDateData
{
	long base;
	long calc[CALC_COUNT];
};
