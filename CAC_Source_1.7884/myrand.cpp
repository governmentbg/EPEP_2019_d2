#define MULTIPLIER      0x015a4e35L
#define INCREMENT       1

static long Seed = 1;

void my_srand(unsigned seed)
{
        Seed = seed;
}

short my_rand()
{
	  Seed = MULTIPLIER * Seed + INCREMENT;
	  return (short) ((int) (Seed >> 16) & 0x7fff);
}
