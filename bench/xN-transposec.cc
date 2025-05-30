// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <vector>

#include "bench/utils.h"
#include "include/xnnpack.h"
#include "src/xnnpack/buffer.h"
#include "src/xnnpack/common.h"
#include "src/xnnpack/hardware-config.h"
#include "src/xnnpack/microfnptr.h"
#include "src/xnnpack/microparams-init.h"
#include "src/xnnpack/transpose.h"
#include <benchmark/benchmark.h>

void transpose(benchmark::State& state, uint64_t arch_flags,
               xnn_transposec_ukernel_fn ukernel, size_t element_size_bits) {
  if (!benchmark::utils::CheckArchFlags(state, arch_flags)) {
    return;
  }
  const size_t element_size = element_size_bits / 8;
  const size_t height = state.range(0);
  const size_t width = state.range(1);
  const size_t tile_hbytes = height * element_size * sizeof(uint8_t);
  const size_t tile_wbytes = width * element_size * sizeof(uint8_t);

  xnnpack::Buffer<uint8_t, XNN_ALLOCATION_ALIGNMENT> x(
      height * width * element_size, xnnpack::XnnExtraBytes);
  xnnpack::Buffer<uint8_t, XNN_ALLOCATION_ALIGNMENT> y(
      height * width * element_size, xnnpack::XnnExtraBytes);
  std::iota(x.begin(), x.end(), 0);

  for (auto _ : state) {
    ukernel(x.data(), y.data(), tile_wbytes, tile_hbytes, width, height);
  }

  const uint64_t cpu_frequency = benchmark::utils::GetCurrentCpuFrequency();
  if (cpu_frequency != 0) {
    state.counters["cpufreq"] = cpu_frequency;
  }
}

static void BenchmarkKernelSize(benchmark::internal::Benchmark* b) {
  b->ArgNames({"height", "width"});
  b->Args({32, 32});
  b->Args({64, 64});
  b->Args({117, 117});
  b->Args({128, 128});
  b->Args({256, 256});
  b->Args({512, 512});
  b->Args({1024, 1024});
}

#define XNN_TRANSPOSE_UKERNEL(arch_flags, ukernel, element_size, ...)     \
  BENCHMARK_CAPTURE(transpose, ukernel, arch_flags,                       \
                    reinterpret_cast<xnn_transposec_ukernel_fn>(ukernel), \
                    element_size)                                         \
      ->Apply(BenchmarkKernelSize)                                        \
      ->UseRealTime();
#include "src/x16-transposec/x16-transposec.inc"
#include "src/x24-transposec/x24-transposec.inc"
#include "src/x32-transposec/x32-transposec.inc"
#include "src/x64-transposec/x64-transposec.inc"
#include "src/x8-transposec/x8-transposec.inc"
#undef XNN_TRANSPOSE_UKERNEL

#ifndef XNNPACK_BENCHMARK_NO_MAIN
XNN_BENCHMARK_MAIN();
#endif
