#include "common.h"

#include "ntl_block.c"

void mbk_check(int error)
{
	switch (error)
	{
		case 0 : break;
		case ENOMEM : out_of_memory();
		case E2BIG : fatal("Прекалено голям блок памет.");
		default : fatal("Грешка при работа с паметта.");
	}
}

void mbk_init(memblock_t *block)
{
	ntl_block_init(block);
#if RANGECHECK
	if (block->data != block->cache)
		fatal("no cache");
#endif  // RANGECHECK
	block->alloc_value = 256;
}
