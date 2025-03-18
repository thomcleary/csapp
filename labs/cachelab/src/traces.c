#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "traces.h"

static enum TraceOp get_traceop(char c) {
  switch (c) {
  case 'I':
    return TRACE_OP_INSTRUCTION_LOAD;
  case 'L':
    return TRACE_OP_LOAD;
  case 'S':
    return TRACE_OP_STORE;
  case 'M':
    return TRACE_OP_MODIFY;
  }

  fprintf(stderr, "Invalid TraceOp [%c]\n", c);
  exit(EXIT_FAILURE);
}

void free_traces(Traces *traces) {
  for (size_t i = 0; i < traces->length; i++) {
    free(traces->value[i].trace);
  }
  free(traces->value);
  free(traces);
}

Traces *read_traces(char *trace_filename) {
  FILE *tracefile = fopen(trace_filename, "r");
  assert(tracefile != NULL && "fopen failed");

  Traces *traces = malloc(sizeof(*traces));
  assert(traces != NULL && "malloc failed");

  traces->length = 0;
  traces->size = BUFSIZ;
  traces->value = malloc(sizeof(*traces->value) * traces->size);
  assert(traces->value != NULL && "malloc failed");

  char *line;
  size_t line_length = 0;
  errno = 0;

  while (getline(&line, &line_length, tracefile) != -1) {
    char op;
    char address[17]; // 64 bit address in hex = 16 chars + 1 null byte

    int matched = sscanf(line, " %c %16s,", &op, address);
    assert(matched == 2 && "sscanf failed");

    errno = 0;
    Trace trace = {.operation = get_traceop(op),
                   .address = strtoull(address, NULL, 16),
                   .trace = strdup(line)};
    assert(errno == 0 && "strtoull failed");
    assert(trace.trace != NULL && "strdup failed");

    if (traces->length == traces->size) {
      traces->size *= 2;
      traces->value =
          realloc(traces->value, sizeof(*traces->value) * traces->size);
      assert(traces->value != NULL && "realloc failed");
    }

    traces->value[traces->length++] = trace;
  }

  assert(errno == 0 && "getline failed");

  free(line);
  fclose(tracefile);

  return traces;
}
