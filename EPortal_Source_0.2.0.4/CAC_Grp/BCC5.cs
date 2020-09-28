public static class BCC5
{
    private class SeedType
    {
        public uint lo = 1;
        public uint hi = 0;
    };

    private static SeedType Seed = new SeedType();
    private const uint MULTIPLIER = 0x015a4e35;
    private const uint INCREMENT = 1;

   
    public static void srand(uint seed)
    {
        Seed.lo = seed;
        Seed.hi = 0;
    }

    public static void srand(int seed)
    {
        Seed.lo = (uint) seed;
        Seed.hi = 0;
    }

    public static int rand()
    {
        Seed.lo = MULTIPLIER * Seed.lo + INCREMENT;
        return (int) ((Seed.lo >> 16) & 0x7fff);
    }
}
