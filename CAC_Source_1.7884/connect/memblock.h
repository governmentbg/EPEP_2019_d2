#define MBKALLOCACHE 0x4C

// 2015:247 LPR: removed step

struct memblock_t
{
	unsigned char *data;
	size_t size;
	size_t bytes;
#if MBKALLOCACHE
	unsigned char cache[MBKALLOCACHE];	// MOD:
#endif  // MBKALLOCACHE
};

void mbk_fatal(const char *message, int errno);	// NEW: non-m.vprintf()
void mbk_oom();	// NEW: CAC

void mbk_init(memblock_t *block);	// NEW: non-alloc
void mbk_resize(memblock_t *block, size_t size);	// MOD: never shrink
void mbk_insert(memblock_t *block, const void *data, size_t offset, size_t size);
void mbk_remove(memblock_t *block, size_t offset, size_t size);
void mbk_append(memblock_t *block, const void *data, size_t size);
void mbk_clear(memblock_t *block);
