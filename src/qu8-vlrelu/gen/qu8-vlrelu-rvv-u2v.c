// clang-format off
// Auto-generated file. Do not edit!
//   Template: src/qs8-vlrelu/rvv.c.in
//   Generator: tools/xngen
//
// Copyright 2024 Imagination Technologies, inc.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <riscv_vector.h>

#include "src/xnnpack/vunary.h"


void xnn_qu8_vlrelu_ukernel__rvv_u2v(
    size_t batch,
    const uint8_t* input,
    uint8_t* output,
    const struct xnn_qu8_lrelu_params* restrict params)
{
  assert(batch != 0);
  assert(batch % sizeof(uint8_t) == 0);
  assert(input != NULL);
  assert(output != NULL);

  const int32_t input_zero_point = params->scalar.input_zero_point;
  const int32_t multiplier_diff = params->scalar.negative_multiplier ^ params->scalar.positive_multiplier;
  const int32_t multiplier_base = params->scalar.positive_multiplier;
  const int32_t bias = (int32_t) (((uint32_t) (int32_t) params->scalar.output_zero_point) << 8) + 128;
  int32_t n = __riscv_vsetvl_e8m2(batch);
  vint32m8_t bias_i32v = __riscv_vmv_v_x_i32m8(bias, n);

  do {
    n = __riscv_vsetvl_e8m2(batch); batch -= n;

    vuint8m2_t in_u8v = __riscv_vle8_v_u8m2(input, n); input += n;
    vuint16m4_t acc_u16v = __riscv_vwsubu_vx_u16m4(in_u8v, input_zero_point, n);
    vint16m4_t acc_i16v = __riscv_vreinterpret_v_u16m4_i16m4(acc_u16v);

    vint32m8_t acc_i32v = __riscv_vwcvt_x_x_v_i32m8(acc_i16v, n);
    vint32m8_t sra_i32v = __riscv_vsra_vx_i32m8(acc_i32v, 31, n);
    vint32m8_t and_i32v = __riscv_vand_vx_i32m8(sra_i32v, multiplier_diff, n);
    vint32m8_t mult_i32v = __riscv_vxor_vx_i32m8(and_i32v, multiplier_base, n);
    acc_i32v = __riscv_vmacc_vv_i32m8(bias_i32v, acc_i32v, mult_i32v, n);

    acc_i32v = __riscv_vmax_vx_i32m8(acc_i32v, 0, n);
    vuint32m8_t out_u32v = __riscv_vreinterpret_v_i32m8_u32m8(acc_i32v);
    vuint16m4_t out_u16v =__riscv_vnclipu_wx_u16m4(out_u32v, 8, __RISCV_VXRM_RDN, n);
    vuint8m2_t out_u8v = __riscv_vnclipu_wx_u8m2(out_u16v, 0, __RISCV_VXRM_RNU, n);
    __riscv_vse8_v_u8m2(output, out_u8v, n); output += n;
  } while (batch != 0);
}
