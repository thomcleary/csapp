#include "sim.h"
#include "cache.h"
#include "traces.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static void print_trace(char *trace) {
  while (isblank(*trace)) {
    trace++;
  }

  while (*trace != '\n') {
    putchar(*trace);
    trace++;
  }

  putchar(' ');
}

static void print_cache_result(CacheResult result) {
  if (result.hit) {
    printf("hit ");
  } else {
    printf("miss ");
    if (result.eviction) {
      printf("eviction ");
    }
  }
}

SimResult run_sim(Cache *cache, Traces *traces, bool verbose) {
  SimResult result = {.hits = 0, .misses = 0, .evictions = 0};

  for (size_t i = 0; i < traces->length; i++) {
    Trace trace = traces->value[i];

    if (trace.operation == TRACE_OP_INSTRUCTION_LOAD) {
      continue;
    }

    if (verbose) {
      print_trace(trace.trace);
    }

    if (trace.operation == TRACE_OP_LOAD) {
      CacheResult load_result = cache_load(cache, trace.address);
      if (verbose) {
        print_cache_result(load_result);
      }

      if (load_result.hit) {
        result.hits++;
      } else {
        result.misses++;
      }
      if (load_result.eviction) {
        result.evictions++;
      }
    } else if (trace.operation == TRACE_OP_STORE) {
      CacheResult store_result = cache_store(cache, trace.address);
      if (verbose) {
        print_cache_result(store_result);
      }

      if (store_result.hit) {
        result.hits++;
      } else {
        result.misses++;
      }
      if (store_result.eviction) {
        result.evictions++;
      }
    } else if (trace.operation == TRACE_OP_MODIFY) {
      CacheResult load_result = cache_load(cache, trace.address);
      CacheResult store_result = cache_store(cache, trace.address);
      if (verbose) {
        print_cache_result(load_result);
        print_cache_result(store_result);
      }

      if (load_result.hit) {
        result.hits++;
      } else {
        result.misses++;
      }
      if (load_result.eviction) {
        result.evictions++;
      }

      if (store_result.hit) {
        result.hits++;
      } else {
        result.misses++;
      }
      if (store_result.eviction) {
        result.evictions++;
      }
    } else {
      fprintf(stderr, "Invalid trace operation [%d]", trace.operation);
      exit(EXIT_FAILURE);
    }

    if (verbose) {
      printf("\n");
    }
  }

  return result;
}
