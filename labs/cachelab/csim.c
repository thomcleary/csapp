#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cachelab.h"
#include "src/cache.h"
#include "src/sim.h"
#include "src/traces.h"

#define NUM_ADDRESS_BITS (sizeof(uint64_t) * 8)

typedef struct SimOptions {
  uint8_t set_index_bits;    // -s
  uint64_t associativity;    // -E
  uint8_t block_offset_bits; // -b
  char *trace_filename;      // -t
  bool verbose;              // -v
} SimOptions;

static void usage() {
  printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
  printf("Options:\n");
  printf("  -h         Print this help message.\n");
  printf("  -v         Optional verbose flag.\n");
  printf("  -s <num>   Number of set index bits.\n");
  printf("  -E <num>   Number of lines per set.\n");
  printf("  -b <num>   Number of block offset bits.\n");
  printf("  -t <file>  Trace file.\n");
  printf("\n");
  printf("Examples:\n");
  printf("linux> ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
  printf("linux> ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

void free_sim_options(SimOptions opts) {
  free(opts.trace_filename);
}

SimOptions get_sim_options(int argc, char *argv[]) {
  SimOptions opts = {.set_index_bits = 0,
                     .associativity = 0,
                     .block_offset_bits = 0,
                     .trace_filename = NULL,
                     .verbose = false};

  const char *optstring = "hvs:E:b:t:";
  int opt;

  while ((opt = getopt(argc, argv, optstring)) != -1) {
    switch (opt) {
    case 'h': {
      usage();
      exit(EXIT_SUCCESS);
    }
    case 'v':
      opts.verbose = true;
      break;
    case 's': {
      errno = 0;
      long s = strtol(optarg, NULL, 10);
      assert(errno == 0 && "strtoul failed");

      if (s >= NUM_ADDRESS_BITS || s < 0) {
        fprintf(stderr, "'-s' must be [0-63]\n");
        break;
      }

      opts.set_index_bits = (uint8_t)s;
      break;
    }
    case 'E': {
      errno = 0;
      long long e = strtoll(optarg, NULL, 10);
      assert(errno == 0 && "strtoull failed");

      if (e <= 0) {
        fprintf(stderr, "'-E' must be greater than 0\n");
        break;
      }

      opts.associativity = (uint64_t)e;
      break;
    }
    case 'b': {
      errno = 0;
      long b = strtol(optarg, NULL, 10);
      assert(errno == 0 && "strtoul failed");

      if (b >= NUM_ADDRESS_BITS || b <= 0) {
        fprintf(stderr, "'-b' must be [1-63]\n");
        break;
      }

      opts.block_offset_bits = (uint8_t)b;
      break;
    }
    case 't': {
      opts.trace_filename = strdup(optarg);
      assert(opts.trace_filename != NULL && "strdup failed");
      break;
    }
    default: { // case '?':
      usage();
      exit(EXIT_FAILURE);
    }
    }
  }

  if (!opts.trace_filename || !opts.associativity || !opts.block_offset_bits) {
    usage();
    exit(EXIT_FAILURE);
  }

  if (opts.set_index_bits + opts.block_offset_bits > NUM_ADDRESS_BITS) {
    fprintf(stderr, "'-s' + '-E' cannot exceed number of address bits [%ld]\n",
            NUM_ADDRESS_BITS);
    usage();
    exit(EXIT_FAILURE);
  }

  return opts;
}

int main(int argc, char *argv[]) {
  SimOptions sim_options = get_sim_options(argc, argv);
  Traces *traces = read_traces(sim_options.trace_filename);
  Cache *cache = cache_new(
      (CacheOptions){.set_index_bits = sim_options.set_index_bits,
                     .associativity = sim_options.associativity,
                     .block_offset_bits = sim_options.block_offset_bits});

  SimResult result = run_sim(cache, traces, sim_options.verbose);

  cache_free(cache);
  free_sim_options(sim_options);
  free_traces(traces);

  printSummary(result.hits, result.misses, result.evictions);

  return 0;
}
