#ifndef TRACES_H
#define TRACES_H

#include <stddef.h>
#include <stdint.h>

enum TraceOp {
  TRACE_OP_INSTRUCTION_LOAD,
  TRACE_OP_LOAD,
  TRACE_OP_STORE,
  TRACE_OP_MODIFY
};

typedef struct Trace {
  enum TraceOp operation;
  uint64_t address;
  char *trace;
} Trace;

typedef struct Traces {
  Trace *value;
  size_t length;
  size_t size;
} Traces;

void free_traces(Traces *traces);
Traces *read_traces(char *trace_filename);

#endif
