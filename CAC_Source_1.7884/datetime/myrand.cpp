#define MULTIPLIER      0x015a4e35L
#define INCREMENT       1

static long Seed = 1;

void my_srand(unsigned seed)
{
        Seed = seed;
}

int my_rand(void)
{
        Seed = MULTIPLIER * Seed + INCREMENT;
        return ((int) (Seed >> 16) & 0x7fff);
}
