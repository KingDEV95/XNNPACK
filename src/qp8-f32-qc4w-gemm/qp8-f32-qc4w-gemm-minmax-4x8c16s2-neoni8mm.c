// Copyright 2024 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <stddef.h>
#include "src/xnnpack/microparams.h"

#if XNN_ENABLE_KLEIDIAI
#include "kai/ukernels/matmul/matmul_clamp_f32_qai8dxp_qsi4cxp/kai_matmul_clamp_f32_qai8dxp4x8_qsi4cxp8x8_4x8x32_neon_i8mm.h"
#endif  // XNN_ENABLE_KLEIDIAI

// Wraps the `kai_run_matmul_clamp_f32_qai8dxp4x8_qsi4cxp8x8_4x8x32_neon_i8mm`
// GEMM microkernel with a name that is compatible with our tooling.
void xnn_qp8_f32_qc4w_gemm_minmax_ukernel_4x8c16s2__neoni8mm(
    size_t m, size_t n, size_t k, const void* lhs_packed,
    const void* rhs_packed, float* dst, size_t dst_stride_row,
    size_t dst_stride_col,
    struct xnn_f32_minmax_params* minmax_params) {
#if XNN_ENABLE_KLEIDIAI
  kai_run_matmul_clamp_f32_qai8dxp4x8_qsi4cxp8x8_4x8x32_neon_i8mm(
      m, n, k, lhs_packed, rhs_packed, dst, dst_stride_row, dst_stride_col,
      minmax_params->scalar.min, minmax_params->scalar.max);
#else
  assert(
      "Calling KleidiAI microkernel wrapper, but XNNPACK was compiled without "
      "`XNN_ENABLE_KLEIDIAI`." && 0);
#endif  // XNN_ENABLE_KLEIDIAI
}
