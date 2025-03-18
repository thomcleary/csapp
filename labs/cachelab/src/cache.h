#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct CacheOptions {
  uint8_t set_index_bits;
  uint64_t associativity;
  uint8_t block_offset_bits;
} CacheOptions;

typedef struct CacheResult {
  bool hit;
  bool eviction;
} CacheResult;

typedef struct Cache Cache;

Cache *cache_new(CacheOptions opts);
void cache_free(Cache *cache);
CacheResult cache_load(Cache *cache, uint64_t address);
CacheResult cache_store(Cache *cache, uint64_t address);

#endif
