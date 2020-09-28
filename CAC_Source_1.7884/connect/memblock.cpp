#include "cac.h"

// Тази модифицирана и съкратена версия на memblock е безплатно лицензирана
// за неограничено използване в проекта "САС - Съдебно Деловодство" на
// "Информационно обслужване" АД клон Варна.

void mbk_fatal(const char *message, int code)
{
	MessageBox(NULL, message, APPNAMENAME, MB_ICONSTOP | MB_OK | MB_TASKMODAL);
	ExitProcess(code);
}

void mbk_oom()
{
	mbk_fatal("Няма достатъчно памет.", ENOMEM);
}

void mbk_init(memblock_t *block)
{
#if MBKALLOCACHE
	block->data = block->cache;
#else  // MBKALLOCACHE
	block->data = NULL;
#endif  // MBKALLOCACHE
	block->size = 0;
	block->bytes = MBKALLOCACHE;
}

void mbk_resize(memblock_t *block, size_t size)
{
	if (size > block->bytes)
	{
		size_t bytes = 256;

		while (bytes < size)
			if ((bytes <<= 1) >= INT_MAX)
				mbk_oom();

#if MBKALLOCACHE
		if (bytes > MBKALLOCACHE)
		{
			unsigned char *data;

			if (block->bytes <= MBKALLOCACHE)
			{
				if ((data = (unsigned char *) malloc(bytes)) == NULL)
					mbk_oom();

				if (block->size > 0)
					memcpy(data, block->data, block->size);
			}
			else if ((data = (unsigned char *) realloc(block->data, bytes)) == NULL)
				mbk_oom();

			block->data = data;
		}
#else  // MBKALLOCACHE
		unsigned char *data = (unsigned char *) realloc(block->data, bytes);

		if (!data)
			mbk_oom();

		block->data = data;
		block->bytes = bytes;
#endif  // MBKALLOCACHE

		block->bytes = bytes;
	}

	block->size = size;
}

void mbk_insert(memblock_t *block, const void *data, size_t offset, size_t size)
{
#if RANGECHECK
	if (offset > block->size)
		mbk_fatal("mbk_insert: range check", EINVAL);
#endif  // RANGECHECK
	mbk_resize(block, block->size + size);

	if (block->size != offset + size)	// 2012:054 LPR: suppress CG errors
		memmove(block->data + offset + size, block->data + offset, block->size - offset - size);

	if (data)
		memcpy(block->data + offset, data, size);
}

void mbk_remove(memblock_t *block, size_t offset, size_t size)
{
#if RANGECHECK
	if (offset > block->size || block->size - offset < size)
		mbk_fatal("mbk_remove: range check", EINVAL);
#endif  // RANGECHECK
	memmove(block->data + offset, block->data + offset + size, block->size - offset - size);
	block->size -= size;
}

void mbk_append(memblock_t *block, const void *data, size_t size)
{
	mbk_insert(block, data, block->size, size);
}

void mbk_clear(memblock_t *block)
{
#if MBKALLOCACHE
	if (block->bytes > MBKALLOCACHE)
	{
		free(block->data);
		block->data = block->cache;
	}
#else  // MBKALLOCACHE
	free(block->data);
	block->data = NULL;
#endif  // MBKALLOCACHE
	block->size = 0;
	block->bytes = MBKALLOCACHE;
}
