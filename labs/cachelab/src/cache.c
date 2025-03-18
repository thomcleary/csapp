#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

typedef struct CacheLine {
  bool valid;
  uint64_t tag;
  uint64_t lru_counter;
} CacheLine;

typedef struct CacheSet {
  CacheLine *lines;
  size_t num_lines;
} CacheSet;

typedef struct Cache {
  CacheSet *sets;
  size_t num_sets;
  uint8_t set_index_bits;
  uint64_t associativity;
  uint8_t block_offset_bits;
} Cache;

void cache_free(Cache *cache) {
  for (size_t i = 0; i < cache->num_sets; i++) {
    free(cache->sets[i].lines);
  }
  free(cache->sets);
  free(cache);
}

Cache *cache_new(CacheOptions opts) {
  Cache *cache = malloc(sizeof(*cache));
  assert(cache != NULL && "malloc failed");

  cache->set_index_bits = opts.set_index_bits;
  cache->associativity = opts.associativity;
  cache->block_offset_bits = opts.block_offset_bits;

  cache->num_sets = 0x1 << cache->set_index_bits; // 2^s
  cache->sets = malloc(sizeof(*cache->sets) * cache->num_sets);
  assert(cache->sets != NULL && "malloc failed");

  for (size_t i = 0; i < cache->num_sets; i++) {
    CacheSet *set = &cache->sets[i];
    set->num_lines = cache->associativity;
    set->lines = malloc(sizeof(*set->lines) * set->num_lines);
    assert(set->lines != NULL && "malloc failed");

    for (size_t j = 0; j < set->num_lines; j++) {
      set->lines[j] = (CacheLine){.valid = false, .tag = 0, .lru_counter = 0};
    }
  }

  return cache;
}

static uint64_t get_set_index(Cache *cache, uint64_t address) {
  const uint8_t num_address_bits = sizeof(address) * 8;
  uint8_t tag_bits =
      num_address_bits - cache->set_index_bits - cache->block_offset_bits;
  uint64_t set_index = address << tag_bits;
  return set_index >> (tag_bits + cache->block_offset_bits);
}

static uint64_t get_tag(Cache *cache, uint64_t address) {
  return address >> (cache->set_index_bits + cache->block_offset_bits);
}

typedef struct LRUBounds {
  uint64_t max, min;
} LRUBounds;

static struct LRUBounds get_lru_bounds(CacheSet *set) {
  CacheLine *line = &set->lines[0];
  LRUBounds bounds = {.max = line->lru_counter, .min = line->lru_counter};

  for (size_t i = 1; i < set->num_lines; i++) {
    CacheLine *line = &set->lines[i];

    if (!line->valid) {
      continue;
    }

    uint64_t lru_counter = line->lru_counter;

    if (lru_counter < bounds.min) {
      bounds.min = lru_counter;
    } else if (lru_counter > bounds.max) {
      bounds.max = lru_counter;
    }
  }

  if (bounds.max == UINT64_MAX) {
    for (size_t i = 0; i < set->num_lines; i++) {
      set->lines[i].lru_counter -= bounds.min;
    }

    bounds.min -= bounds.min;
    bounds.max -= bounds.max;
  }

  return bounds;
}

CacheResult cache_load(Cache *cache, uint64_t address) {
  uint64_t set_index = get_set_index(cache, address);
  uint64_t tag = get_tag(cache, address);

  assert(set_index < cache->num_sets && "invalid set index");
  CacheSet *set = &cache->sets[set_index];

  CacheLine *hit_line = NULL;

  bool free_line = false;
  size_t free_line_index = 0;

  for (size_t i = 0; i < set->num_lines; i++) {
    CacheLine *line = &set->lines[i];

    if (!line->valid) {
      if (!free_line) {
        free_line = true;
        free_line_index = i;
      }
      continue;
    }

    if (line->tag == tag) {
      hit_line = line;
    }
  }

  LRUBounds lru_bounds = get_lru_bounds(set);

  if (hit_line) {
    hit_line->lru_counter = lru_bounds.max + 1;
    return (CacheResult){.hit = true, .eviction = false};
  }

  if (free_line) {
    CacheLine *line = &set->lines[free_line_index];
    line->valid = true;
    line->tag = tag;
    line->lru_counter = lru_bounds.max + 1;
    return (CacheResult){.hit = false, .eviction = false};
  }

  CacheLine *lru_line = NULL;
  for (size_t i = 0; i < set->num_lines; i++) {
    CacheLine *line = &set->lines[i];

    if (line->valid && line->lru_counter == lru_bounds.min) {
      lru_line = line;
      break;
    }
  }

  assert(lru_line != NULL && "failed to find line with min LRU counter");

  lru_line->tag = tag;
  lru_line->lru_counter = lru_bounds.max + 1;
  return (CacheResult){.hit = false, .eviction = true};
}

CacheResult cache_store(Cache *cache, uint64_t address) {
  // Simulation just requires checking if already in cache or not, and possible
  // eviction if miss
  return cache_load(cache, address);
}
