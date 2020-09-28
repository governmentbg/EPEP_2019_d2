/*
  Copyright (c) 2016, Dimitar Toshkov Zhekov <dimitar.zhekov@gmail.com>
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
  
  THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "ntl_block.h"

size_t ntl_block_adjust_size_power2(const ntl_block *block, size_t size)
{
	size_t bytes;

	NTL_ASSERT(block != NULL);
	NTL_ASSERT(block->alloc_value != 0);

	if (size == 0)
		bytes = 0;
	else
	{
		bytes = block->alloc_value;

		while (bytes < size)
		{
			if (bytes > (block->limit >> 1))
			{
				bytes = block->limit;
				break;
			}

			bytes <<= 1;
		}
	}

	return bytes;
}

size_t ntl_block_adjust_size_units(const ntl_block *block, size_t size)
{
	size_t bytes;

	NTL_ASSERT(block != NULL);
	NTL_ASSERT(block->alloc_value != 0);

	if (size == 0)
		bytes = 0;
	else
	{
		size_t units = (size - 1) / block->alloc_value + 1;

		if (block->limit / block->alloc_value >= units)
			bytes = units * block->alloc_value;
		else
			bytes = block->limit;
	}

	return bytes;
}

#ifndef NTL_BLOCK_GLOBAL_ALLOC
static void *ntl_block_default_alloc_data(const ntl_block *block, size_t bytes)
{
	(void) block;
	return malloc(bytes);
}

static void *ntl_block_default_realloc_data(const ntl_block *block, size_t bytes)
{
	return realloc(block->data, bytes);
}

static void ntl_block_default_free_data(const ntl_block *block)
{
	free(block->data);
}
#endif  /* GLOBAL_ALLOC */

void *ntl_block_realloc_via_alloc(const ntl_block *block, size_t bytes)
{
	void *data = ntl_block_alloc_data(block, bytes);

	NTL_ASSERT(block != NULL);
	NTL_ASSERT(bytes <= NTL_BLOCK_LIMIT);

	if (data != NULL && block->size != 0)
		memcpy(data, block->data, block->size < bytes ? block->size : bytes);

	if (data != NULL || bytes == 0)
		ntl_block_free_data(block);

	return data;
}

void ntl_block_init_common(ntl_block *block)
{
	NTL_ASSERT(block != NULL);
	block->adjust_size = ntl_block_adjust_size_power2;
	block->alloc_value = 64;
	block->limit = NTL_BLOCK_LIMIT;
	block->try_exact = 0;
	block->shrink = 0;
#if NTL_BLOCK_CACHE_SIZE
	block->data = block->cache;
	block->size = 0;
	block->bytes = NTL_BLOCK_CACHE_SIZE;
#else
	block->data = NULL;
	block->size = 0;
	block->bytes = 0;
#endif  /* CACHE_SIZE */
}

#ifndef NTL_BLOCK_GLOBAL_ALLOC
void ntl_block_init_calloc(ntl_block *block)
{
	ntl_block_init_common(block);
	block->alloc_data = ntl_block_default_alloc_data;
	block->realloc_data = ntl_block_default_realloc_data;
	block->free_data = ntl_block_default_free_data;
}
#endif  /* GLOBAL_ALLOC */

#if NTL_BLOCK_CACHE_SIZE
static unsigned char *ntl_block_realloc_cache(const ntl_block *block, size_t bytes)
{
	void *data;

	if (block->bytes > NTL_BLOCK_CACHE_SIZE)
		data = ntl_block_realloc_data(block, bytes);
	else if ((data = ntl_block_alloc_data(block, bytes)) != NULL)
		memcpy(data, block->data, block->size);

	return (unsigned char *) data;
}
#else
#define ntl_block_realloc_cache(block, bytes) ((unsigned char *) ntl_block_realloc_data(block, bytes))
#endif  /* CACHE_SIZE */

int ntl_block_resize(ntl_block *block, size_t size)
{
	int enlarge = size > block->bytes;

	NTL_ASSERT(block != NULL);
	NTL_ASSERT(size <= block->limit);

	if (enlarge || block->shrink)
	{
		size_t bytes;

		if (size > block->limit)
			return E2BIG;

		bytes = ntl_block_adjust_size(block, size);
		NTL_ASSERT(bytes >= size && bytes <= NTL_BLOCK_LIMIT);

	#if NTL_BLOCK_CACHE_SIZE
		if (size <= NTL_BLOCK_CACHE_SIZE)
		{
			if (bytes <= NTL_BLOCK_CACHE_SIZE && block->bytes > NTL_BLOCK_CACHE_SIZE)
			{
				memcpy(block->cache, block->data, size);
				ntl_block_free_data(block);
				block->data = block->cache;
				block->bytes = NTL_BLOCK_CACHE_SIZE;
			}
		}
		else
	#endif  /* CACHE_SIZE */
		if (bytes != block->bytes)
		{
			unsigned char *data = ntl_block_realloc_cache(block, bytes);

			if (data != NULL || bytes == 0 || (enlarge && block->try_exact && size < bytes &&
				(data = ntl_block_realloc_cache(block, bytes = size)) != NULL))
			{
				block->data = data;
				block->bytes = bytes;
			}
			else if (enlarge)
				return ENOMEM;
		}
	}

	block->size = size;
	return 0;
}

int ntl_block_insert(ntl_block *block, const void *data, size_t offset, size_t size)
{
	int result;

	NTL_ASSERT(block != NULL);
	NTL_ASSERT(offset <= block->size);
	NTL_ASSERT(size <= block->limit);

	if (block->limit - block->size < size)
		result = E2BIG;
	else if ((result = ntl_block_resize(block, block->size + size)) == 0)
	{
		memmove(block->data + offset + size, block->data + offset, block->size - offset - size);

		if (data != NULL)
			memcpy(block->data + offset, data, size);
	}

	return result;
}

int ntl_block_append(ntl_block *block, const void *data, size_t size)
{
	size_t old_size = block->size;
	int result;

	NTL_ASSERT(block != NULL);
	NTL_ASSERT(size <= block->limit);

	if (block->limit - old_size < size)
		result = E2BIG;
	else if ((result = ntl_block_resize(block, old_size + size)) == 0 && data != NULL)
		memcpy(block->data + old_size, data, size);

	return result;
}

void ntl_block_remove(ntl_block *block, size_t offset, size_t size, void *data)
{
	NTL_ASSERT(block != NULL);
	NTL_ASSERT(offset <= block->size);
	NTL_ASSERT(block->size - offset >= size);

	if (data != NULL)
		memcpy(data, block->data + offset, size);

	memmove(block->data + offset, block->data + offset + size, block->size - offset - size);
	ntl_block_resize(block, block->size - size);
}

void ntl_block_clear(ntl_block *block)
{
	NTL_ASSERT(block != NULL);
#if NTL_BLOCK_CACHE_SIZE
	if (block->bytes > NTL_BLOCK_CACHE_SIZE)
	{
		ntl_block_free_data(block);
		block->data = block->cache;
	}

	block->size = 0;
	block->bytes = NTL_BLOCK_CACHE_SIZE;
#else
	ntl_block_free_data(block);
	block->data = NULL;
	block->size = 0;
	block->bytes = 0;
#endif  /* CACHE_SIZE */
}

#ifdef NTL_BLOCK_TEST
#include <assert.h>

int main(void)
{
	ntl_block block;
	unsigned char h;

	ntl_block_init(&block);
	block.alloc_value = 12;
	block.shrink = 1;

	assert(ntl_block_insert(&block, "The quick brown ", 0, 16) == 0);
#if NTL_BLOCK_CACHE_SIZE
	assert(block.data == block.cache);
#endif
	assert(ntl_block_append(&block, "fox jumps over the hazy dog.", 29) == 0);
	assert(block.bytes == 48);
#if NTL_BLOCK_CACHE_SIZE
	assert(block.data != block.cache);
#endif
	assert(strcmp((const char *) block.data, "The quick brown fox jumps over the hazy dog.") == 0);
	ntl_block_remove(&block, 35, 1, &h);
	assert(h == 'h');
	assert(strcmp((const char *) block.data, "The quick brown fox jumps over the azy dog.") == 0);
	assert(ntl_block_insert(&block, "l", 35, 1) == 0);
	assert(strcmp((const char *) block.data, "The quick brown fox jumps over the lazy dog.") == 0);
	assert(ntl_block_resize(&block, 24) == 0);
	assert(block.bytes == 24);
	assert(memcmp((const char *) block.data, "The quick brown fox jump", 24) == 0);

#if NTL_BLOCK_CACHE_SIZE
	assert(ntl_block_resize(&block, 15) == 0);
	assert(block.bytes == 24);
	assert(memcmp(block.data, "The quick brown", 15) == 0);
	assert(ntl_block_resize(&block, 12) == 0);
	assert(block.bytes == NTL_BLOCK_CACHE_SIZE);
	assert(block.data == block.cache);
	assert(ntl_block_resize(&block, 12) == 0);
	assert(memcmp(block.data, "The quick br", 12) == 0);
#endif  /* CACHE_SIZE */

	ntl_block_clear(&block);
	return 0;
}
#endif
