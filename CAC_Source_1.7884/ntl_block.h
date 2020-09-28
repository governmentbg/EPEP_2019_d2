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

#ifndef NTL_BLOCK_H

#include <stddef.h>  /* size_t */

#ifndef NTL_CONFIG_H
#include "ntl_config.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ntl_block ntl_block;

typedef struct _ntl_block
{
	size_t (*adjust_size)(const ntl_block *block, size_t size);
#ifndef NTL_BLOCK_GLOBAL_ALLOC
	void *(*alloc_data)(const ntl_block *block, size_t bytes);
	void *(*realloc_data)(const ntl_block *block, size_t bytes);
	void (*free_data)(const ntl_block *block);
#endif
	unsigned char *data;
	size_t size;
	size_t bytes;
	size_t limit;
	size_t alloc_value;
	unsigned try_exact : 1;
	unsigned shrink : 1;
	unsigned alloc_flags : 4;
#if NTL_BLOCK_CACHE_SIZE
	unsigned char cache[NTL_BLOCK_CACHE_SIZE];
#endif
} ntl_block;

size_t ntl_block_adjust_size_power2(const ntl_block *block, size_t size);
size_t ntl_block_adjust_size_units(const ntl_block *block, size_t size);

inline size_t ntl_block_adjust_size(const ntl_block *block, size_t size) { return block->adjust_size(block, size); }
#ifndef NTL_BLOCK_GLOBAL_ALLOC
inline void *ntl_block_alloc_data(const ntl_block *block, size_t bytes) { return block->alloc_data(block, bytes); }
inline void *ntl_block_realloc_data(const ntl_block *block, size_t bytes) { return block->realloc_data(block, bytes); }
inline void ntl_block_free_data(const ntl_block *block) { block->free_data(block); }
#endif
void *ntl_block_realloc_via_alloc(const ntl_block *block, size_t bytes);

void ntl_block_init_common(ntl_block *block);
#ifndef NTL_BLOCK_GLOBAL_ALLOC
void ntl_block_init_calloc(ntl_block *block);
#endif

inline unsigned char *ntl_block_data(ntl_block *block) { return block->data; }
inline size_t ntl_block_size(ntl_block *block) { return block->size; }
#ifndef ntl_block_init
#ifndef NTL_BLOCK_GLOBAL_ALLOC
#	define ntl_block_init ntl_block_init_calloc
#else
#	define ntl_block_init ntl_block_init_common
#endif  /* GLOBAL_ALLOC */
#endif  /* block_init */
int ntl_block_resize(ntl_block *block, size_t size);
int ntl_block_insert(ntl_block *block, const void *data, size_t offset, size_t size);
int ntl_block_append(ntl_block *block, const void *data, size_t size);
void ntl_block_remove(ntl_block *block, size_t offset, size_t size, void *data);
void ntl_block_clear(ntl_block *block);

#ifdef __cplusplus
}
#endif

#define NTL_BLOCK_H 1
#endif
