// clang-format off
// Auto-generated file. Do not edit!
//   Template: src/f32-vbinary/vopc-rvv.c.in
//   Generator: tools/xngen
//
// Copyright 2023 SiFive, Inc.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <riscv_vector.h>

#include "src/xnnpack/common.h"
#include "src/xnnpack/vbinary.h"


void xnn_f32_vrdivc_ukernel__rvv_u8v(
    size_t batch,
    const float* input_a,
    const float* input_b,
    float* output,
    const struct xnn_f32_default_params* restrict params)
{
  assert(batch != 0);
  assert(batch % sizeof(float) == 0);
  assert(input_a != NULL);
  assert(input_b != NULL);
  assert(output != NULL);

  const float b = *input_b;
  size_t n = batch >> 2;

  do {
    size_t vl = __riscv_vsetvl_e32m8(n);
    n -= vl;
    vfloat32m8_t va = __riscv_vle32_v_f32m8(input_a, vl);
    input_a += vl;
    vfloat32m8_t vacc = __riscv_vfrdiv_vf_f32m8(va, b, vl);
    __riscv_vse32_v_f32m8(output, vacc, vl);
    output += vl;
  } while (n > 0);
}
