#ifndef SIM_H
#define SIM_H

#include <stdbool.h>
#include <stdint.h>

#include "cache.h"
#include "traces.h"

typedef struct SimResult {
  uint64_t hits, misses, evictions;
} SimResult;

SimResult run_sim(Cache *cache, Traces *traces, bool verbose);

#endif
