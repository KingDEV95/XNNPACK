// Auto-generated file. Do not edit!
//   Template: src/f32-velu/wasmsimd-rr2-p6.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <wasm_simd128.h>

#include <xnnpack/vunary.h>
#include <xnnpack/common.h>


void xnn_f32_velu_ukernel__wasmrelaxedsimd_fma_rr2_p6_x24(
    size_t batch,
    const float* input,
    float* output,
    const union xnn_f32_elu_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  assert(batch != 0);
  assert(batch % sizeof(float) == 0);
  assert(input != NULL);
  assert(output != NULL);

  const v128_t vprescale = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.prescale);
  const v128_t valpha = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.alpha);
  const v128_t vbeta = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.beta);
  const v128_t vsat_cutoff = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.sat_cutoff);
  const v128_t vmagic_bias = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.magic_bias);
  const v128_t vlog2e = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.log2e);
  const v128_t vminus_ln2_hi = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.minus_ln2_hi);
  const v128_t vminus_ln2_lo = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.minus_ln2_lo);
  const v128_t vc6 = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.c6);
  const v128_t vc5 = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.c5);
  const v128_t vc4 = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.c4);
  const v128_t vc3 = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.c3);
  const v128_t vc2 = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.c2);
  const v128_t vone = wasm_v128_load64_splat(params->wasmsimd_rr2_p6.one);

  for (; batch >= 24 * sizeof(float); batch -= 24 * sizeof(float)) {
    v128_t vx0123 = wasm_v128_load(input);
    v128_t vx4567 = wasm_v128_load(input + 4);
    v128_t vx89AB = wasm_v128_load(input + 8);
    v128_t vxCDEF = wasm_v128_load(input + 12);
    v128_t vxGHIJ = wasm_v128_load(input + 16);
    v128_t vxKLMN = wasm_v128_load(input + 20);
    input += 24;

    const v128_t vz0123 = __builtin_wasm_relaxed_max_f32x4(vsat_cutoff, wasm_f32x4_mul(vx0123, vprescale));
    const v128_t vz4567 = __builtin_wasm_relaxed_max_f32x4(vsat_cutoff, wasm_f32x4_mul(vx4567, vprescale));
    const v128_t vz89AB = __builtin_wasm_relaxed_max_f32x4(vsat_cutoff, wasm_f32x4_mul(vx89AB, vprescale));
    const v128_t vzCDEF = __builtin_wasm_relaxed_max_f32x4(vsat_cutoff, wasm_f32x4_mul(vxCDEF, vprescale));
    const v128_t vzGHIJ = __builtin_wasm_relaxed_max_f32x4(vsat_cutoff, wasm_f32x4_mul(vxGHIJ, vprescale));
    const v128_t vzKLMN = __builtin_wasm_relaxed_max_f32x4(vsat_cutoff, wasm_f32x4_mul(vxKLMN, vprescale));

    v128_t vn0123 = __builtin_wasm_fma_f32x4(vmagic_bias, vz0123, vlog2e);
    v128_t vn4567 = __builtin_wasm_fma_f32x4(vmagic_bias, vz4567, vlog2e);
    v128_t vn89AB = __builtin_wasm_fma_f32x4(vmagic_bias, vz89AB, vlog2e);
    v128_t vnCDEF = __builtin_wasm_fma_f32x4(vmagic_bias, vzCDEF, vlog2e);
    v128_t vnGHIJ = __builtin_wasm_fma_f32x4(vmagic_bias, vzGHIJ, vlog2e);
    v128_t vnKLMN = __builtin_wasm_fma_f32x4(vmagic_bias, vzKLMN, vlog2e);

    v128_t vs0123 = wasm_i32x4_shl(vn0123, 23);
    v128_t vs4567 = wasm_i32x4_shl(vn4567, 23);
    v128_t vs89AB = wasm_i32x4_shl(vn89AB, 23);
    v128_t vsCDEF = wasm_i32x4_shl(vnCDEF, 23);
    v128_t vsGHIJ = wasm_i32x4_shl(vnGHIJ, 23);
    v128_t vsKLMN = wasm_i32x4_shl(vnKLMN, 23);

    vn0123 = wasm_f32x4_sub(vn0123, vmagic_bias);
    vn4567 = wasm_f32x4_sub(vn4567, vmagic_bias);
    vn89AB = wasm_f32x4_sub(vn89AB, vmagic_bias);
    vnCDEF = wasm_f32x4_sub(vnCDEF, vmagic_bias);
    vnGHIJ = wasm_f32x4_sub(vnGHIJ, vmagic_bias);
    vnKLMN = wasm_f32x4_sub(vnKLMN, vmagic_bias);

    v128_t vt0123 = __builtin_wasm_fma_f32x4(vz0123, vn0123, vminus_ln2_hi);
    v128_t vt4567 = __builtin_wasm_fma_f32x4(vz4567, vn4567, vminus_ln2_hi);
    v128_t vt89AB = __builtin_wasm_fma_f32x4(vz89AB, vn89AB, vminus_ln2_hi);
    v128_t vtCDEF = __builtin_wasm_fma_f32x4(vzCDEF, vnCDEF, vminus_ln2_hi);
    v128_t vtGHIJ = __builtin_wasm_fma_f32x4(vzGHIJ, vnGHIJ, vminus_ln2_hi);
    v128_t vtKLMN = __builtin_wasm_fma_f32x4(vzKLMN, vnKLMN, vminus_ln2_hi);

    vt0123 = __builtin_wasm_fma_f32x4(vt0123, vn0123, vminus_ln2_lo);
    vt4567 = __builtin_wasm_fma_f32x4(vt4567, vn4567, vminus_ln2_lo);
    vt89AB = __builtin_wasm_fma_f32x4(vt89AB, vn89AB, vminus_ln2_lo);
    vtCDEF = __builtin_wasm_fma_f32x4(vtCDEF, vnCDEF, vminus_ln2_lo);
    vtGHIJ = __builtin_wasm_fma_f32x4(vtGHIJ, vnGHIJ, vminus_ln2_lo);
    vtKLMN = __builtin_wasm_fma_f32x4(vtKLMN, vnKLMN, vminus_ln2_lo);

    v128_t vp0123 = __builtin_wasm_fma_f32x4(vc5, vc6, vt0123);
    v128_t vp4567 = __builtin_wasm_fma_f32x4(vc5, vc6, vt4567);
    v128_t vp89AB = __builtin_wasm_fma_f32x4(vc5, vc6, vt89AB);
    v128_t vpCDEF = __builtin_wasm_fma_f32x4(vc5, vc6, vtCDEF);
    v128_t vpGHIJ = __builtin_wasm_fma_f32x4(vc5, vc6, vtGHIJ);
    v128_t vpKLMN = __builtin_wasm_fma_f32x4(vc5, vc6, vtKLMN);

    vp0123 = __builtin_wasm_fma_f32x4(vc4, vp0123, vt0123);
    vp4567 = __builtin_wasm_fma_f32x4(vc4, vp4567, vt4567);
    vp89AB = __builtin_wasm_fma_f32x4(vc4, vp89AB, vt89AB);
    vpCDEF = __builtin_wasm_fma_f32x4(vc4, vpCDEF, vtCDEF);
    vpGHIJ = __builtin_wasm_fma_f32x4(vc4, vpGHIJ, vtGHIJ);
    vpKLMN = __builtin_wasm_fma_f32x4(vc4, vpKLMN, vtKLMN);

    vp0123 = __builtin_wasm_fma_f32x4(vc3, vp0123, vt0123);
    vp4567 = __builtin_wasm_fma_f32x4(vc3, vp4567, vt4567);
    vp89AB = __builtin_wasm_fma_f32x4(vc3, vp89AB, vt89AB);
    vpCDEF = __builtin_wasm_fma_f32x4(vc3, vpCDEF, vtCDEF);
    vpGHIJ = __builtin_wasm_fma_f32x4(vc3, vpGHIJ, vtGHIJ);
    vpKLMN = __builtin_wasm_fma_f32x4(vc3, vpKLMN, vtKLMN);

    vp0123 = __builtin_wasm_fma_f32x4(vc2, vp0123, vt0123);
    vp4567 = __builtin_wasm_fma_f32x4(vc2, vp4567, vt4567);
    vp89AB = __builtin_wasm_fma_f32x4(vc2, vp89AB, vt89AB);
    vpCDEF = __builtin_wasm_fma_f32x4(vc2, vpCDEF, vtCDEF);
    vpGHIJ = __builtin_wasm_fma_f32x4(vc2, vpGHIJ, vtGHIJ);
    vpKLMN = __builtin_wasm_fma_f32x4(vc2, vpKLMN, vtKLMN);

    vp0123 = wasm_f32x4_mul(vp0123, vt0123);
    vp4567 = wasm_f32x4_mul(vp4567, vt4567);
    vp89AB = wasm_f32x4_mul(vp89AB, vt89AB);
    vpCDEF = wasm_f32x4_mul(vpCDEF, vtCDEF);
    vpGHIJ = wasm_f32x4_mul(vpGHIJ, vtGHIJ);
    vpKLMN = wasm_f32x4_mul(vpKLMN, vtKLMN);

    vt0123 = wasm_f32x4_mul(vt0123, vs0123);
    vs0123 = wasm_f32x4_sub(vs0123, vone);
    vt4567 = wasm_f32x4_mul(vt4567, vs4567);
    vs4567 = wasm_f32x4_sub(vs4567, vone);
    vt89AB = wasm_f32x4_mul(vt89AB, vs89AB);
    vs89AB = wasm_f32x4_sub(vs89AB, vone);
    vtCDEF = wasm_f32x4_mul(vtCDEF, vsCDEF);
    vsCDEF = wasm_f32x4_sub(vsCDEF, vone);
    vtGHIJ = wasm_f32x4_mul(vtGHIJ, vsGHIJ);
    vsGHIJ = wasm_f32x4_sub(vsGHIJ, vone);
    vtKLMN = wasm_f32x4_mul(vtKLMN, vsKLMN);
    vsKLMN = wasm_f32x4_sub(vsKLMN, vone);

    vp0123 = __builtin_wasm_fma_f32x4(vt0123, vp0123, vt0123);
    vp4567 = __builtin_wasm_fma_f32x4(vt4567, vp4567, vt4567);
    vp89AB = __builtin_wasm_fma_f32x4(vt89AB, vp89AB, vt89AB);
    vpCDEF = __builtin_wasm_fma_f32x4(vtCDEF, vpCDEF, vtCDEF);
    vpGHIJ = __builtin_wasm_fma_f32x4(vtGHIJ, vpGHIJ, vtGHIJ);
    vpKLMN = __builtin_wasm_fma_f32x4(vtKLMN, vpKLMN, vtKLMN);

    const v128_t ve0123 = wasm_f32x4_mul(valpha, wasm_f32x4_add(vp0123, vs0123));
    const v128_t ve4567 = wasm_f32x4_mul(valpha, wasm_f32x4_add(vp4567, vs4567));
    const v128_t ve89AB = wasm_f32x4_mul(valpha, wasm_f32x4_add(vp89AB, vs89AB));
    const v128_t veCDEF = wasm_f32x4_mul(valpha, wasm_f32x4_add(vpCDEF, vsCDEF));
    const v128_t veGHIJ = wasm_f32x4_mul(valpha, wasm_f32x4_add(vpGHIJ, vsGHIJ));
    const v128_t veKLMN = wasm_f32x4_mul(valpha, wasm_f32x4_add(vpKLMN, vsKLMN));

    const v128_t vsignm0123 = wasm_i32x4_shr(vx0123, 31);
    vx0123 = wasm_f32x4_mul(vx0123, vbeta);
    const v128_t vsignm4567 = wasm_i32x4_shr(vx4567, 31);
    vx4567 = wasm_f32x4_mul(vx4567, vbeta);
    const v128_t vsignm89AB = wasm_i32x4_shr(vx89AB, 31);
    vx89AB = wasm_f32x4_mul(vx89AB, vbeta);
    const v128_t vsignmCDEF = wasm_i32x4_shr(vxCDEF, 31);
    vxCDEF = wasm_f32x4_mul(vxCDEF, vbeta);
    const v128_t vsignmGHIJ = wasm_i32x4_shr(vxGHIJ, 31);
    vxGHIJ = wasm_f32x4_mul(vxGHIJ, vbeta);
    const v128_t vsignmKLMN = wasm_i32x4_shr(vxKLMN, 31);
    vxKLMN = wasm_f32x4_mul(vxKLMN, vbeta);

    const v128_t vy0123 = wasm_v128_bitselect(ve0123, vx0123, vsignm0123);
    const v128_t vy4567 = wasm_v128_bitselect(ve4567, vx4567, vsignm4567);
    const v128_t vy89AB = wasm_v128_bitselect(ve89AB, vx89AB, vsignm89AB);
    const v128_t vyCDEF = wasm_v128_bitselect(veCDEF, vxCDEF, vsignmCDEF);
    const v128_t vyGHIJ = wasm_v128_bitselect(veGHIJ, vxGHIJ, vsignmGHIJ);
    const v128_t vyKLMN = wasm_v128_bitselect(veKLMN, vxKLMN, vsignmKLMN);

    wasm_v128_store(output, vy0123);
    wasm_v128_store(output + 4, vy4567);
    wasm_v128_store(output + 8, vy89AB);
    wasm_v128_store(output + 12, vyCDEF);
    wasm_v128_store(output + 16, vyGHIJ);
    wasm_v128_store(output + 20, vyKLMN);
    output += 24;
  }
  for (; batch >= 4 * sizeof(float); batch -= 4 * sizeof(float)) {
    v128_t vx = wasm_v128_load(input);
    input += 4;

    const v128_t vz = __builtin_wasm_relaxed_max_f32x4(vsat_cutoff, wasm_f32x4_mul(vx, vprescale));

    v128_t vn = __builtin_wasm_fma_f32x4(vmagic_bias, vz, vlog2e);
    v128_t vs = wasm_i32x4_shl(vn, 23);
    vn = wasm_f32x4_sub(vn, vmagic_bias);

    v128_t vt = __builtin_wasm_fma_f32x4(vz, vn, vminus_ln2_hi);
    vt = __builtin_wasm_fma_f32x4(vt, vn, vminus_ln2_lo);

    v128_t vp = __builtin_wasm_fma_f32x4(vc5, vc6, vt);
    vp = __builtin_wasm_fma_f32x4(vc4, vp, vt);
    vp = __builtin_wasm_fma_f32x4(vc3, vp, vt);
    vp = __builtin_wasm_fma_f32x4(vc2, vp, vt);
    vp = wasm_f32x4_mul(vp, vt);

    vt = wasm_f32x4_mul(vt, vs);
    vs = wasm_f32x4_sub(vs, vone);
    vp = __builtin_wasm_fma_f32x4(vt, vp, vt);
    const v128_t ve = wasm_f32x4_mul(wasm_f32x4_add(vp, vs), valpha);

    const v128_t vsignm = wasm_i32x4_shr(vx, 31);
    vx = wasm_f32x4_mul(vx, vbeta);
    const v128_t vy = wasm_v128_bitselect(ve, vx, vsignm);

    wasm_v128_store(output, vy);
    output += 4;
  }
  if XNN_UNLIKELY(batch != 0) {
    v128_t vx = wasm_v128_load(input);

    const v128_t vz = __builtin_wasm_relaxed_max_f32x4(wasm_f32x4_mul(vx, vprescale), vsat_cutoff);

    v128_t vn = __builtin_wasm_fma_f32x4(vmagic_bias, vz, vlog2e);
    v128_t vs = wasm_i32x4_shl(vn, 23);
    vn = wasm_f32x4_sub(vn, vmagic_bias);

    v128_t vt = __builtin_wasm_fma_f32x4(vz, vn, vminus_ln2_hi);
    vt = __builtin_wasm_fma_f32x4(vt, vn, vminus_ln2_lo);

    v128_t vp = __builtin_wasm_fma_f32x4(vc5, vc6, vt);
    vp = __builtin_wasm_fma_f32x4(vc4, vp, vt);
    vp = __builtin_wasm_fma_f32x4(vc3, vp, vt);
    vp = __builtin_wasm_fma_f32x4(vc2, vp, vt);
    vp = wasm_f32x4_mul(vp, vt);

    vt = wasm_f32x4_mul(vt, vs);
    vs = wasm_f32x4_sub(vs, vone);
    vp = __builtin_wasm_fma_f32x4(vt, vp, vt);
    const v128_t ve = wasm_f32x4_mul(wasm_f32x4_add(vp, vs), valpha);

    const v128_t vsignm = wasm_i32x4_shr(vx, 31);
    vx = wasm_f32x4_mul(vx, vbeta);
    v128_t vy = wasm_v128_bitselect(ve, vx, vsignm);

    if (batch & (2 * sizeof(float))) {
      wasm_v128_store64_lane(output, vy, 0);
      vy = wasm_v64x2_shuffle(vy, vy, 1, 1);
      output += 2;
    }
    if (batch & (1 * sizeof(float))) {
      wasm_v128_store32_lane(output, vy, 0);
    }
  }
}
