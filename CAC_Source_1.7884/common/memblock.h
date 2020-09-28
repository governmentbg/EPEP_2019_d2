#define NTL_CONFIG_H 1
#if TESTVER
#define NTL_DEBUG 1
#define NTL_ASSERT(expr) ((expr) ? (void) 0 : fatal("Assert: %s, %s:%d", #expr, __FILE__, __LINE__))
#else
#define NTL_ASSERT(expr)
#endif

#define NTL_BLOCK_LIMIT 0x40000000u
#define NTL_BLOCK_GLOBAL_ALLOC 1
#define NTL_BLOCK_CACHE_SIZE 0x4C

#include "ntl_block.h"
#define ntl_block_alloc_data(block, bytes) malloc(bytes)
#define ntl_block_realloc_data(block, bytes) realloc((block)->data, bytes)
#define ntl_block_free_data(block) free((block)->data)

typedef ntl_block memblock_t;

void mbk_check(int result);
void mbk_init(memblock_t *block);
#define mbk_resize(block, size) mbk_check(ntl_block_resize(block, size))
#define mbk_insert(block, data, offset, size) mbk_check(ntl_block_insert(block, data, offset, size))
#define mbk_append(block, data, size) mbk_check(ntl_block_append(block, data, size))
#define mbk_remove(block, offset, size) ntl_block_remove(block, offset, size, NULL)
#define mbk_clear(block) ntl_block_clear(block)
