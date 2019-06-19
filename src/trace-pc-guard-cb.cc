// trace-pc-guard-cb.cc
// https://clang.llvm.org/docs/SanitizerCoverage.html
#include "coverageReporter.h"
#include <sanitizer/coverage_interface.h>
#include <stdio.h>
#include <string>

extern CoverageReporter coverageReporter;
extern bool outputMessages;

extern "C" void __sanitizer_cov_trace_pc_guard_init(uint32_t *start,
                                                    uint32_t *stop) {
  static uint64_t N;
  if (start == stop || *start)
    return;
  printf("INIT: %p %p\n", start, stop);
  for (uint32_t *x = start; x < stop; x++) {
    *x = ++N;
  }
}

extern "C" void __sanitizer_cov_trace_pc_guard(uint32_t *guard) {
  if (!*guard)
    return;
  void *PC = __builtin_return_address(0);
  char PcDescr[1024];
  __sanitizer_symbolize_pc(PC, "%F %L", PcDescr, sizeof(PcDescr));
  if (coverageReporter.recordingCoverage) {
    coverageReporter.addPCForSequence(std::string(PcDescr));
    if (outputMessages)
      printf("adding to the list: ");
  }
  if (outputMessages)
    printf("guard: %p %x PC %s\n", guard, *guard, PcDescr);
}
