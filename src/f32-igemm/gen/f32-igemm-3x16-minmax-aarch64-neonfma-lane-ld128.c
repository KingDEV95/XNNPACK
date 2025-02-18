// Auto-generated file. Do not edit!
//   Template: src/f32-igemm/neon-ld128.c.in
//   Generator: tools/xngen
//
// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.


#include <assert.h>

#include <arm_neon.h>

#include "xnnpack/igemm.h"


void xnn_f32_igemm_minmax_ukernel_3x16__aarch64_neonfma_lane_ld128(
    size_t mr,
    size_t nc,
    size_t kc,
    size_t ks,
    const float** restrict a,
    const float* restrict w,
    float* restrict c,
    size_t cm_stride,
    size_t cn_stride,
    size_t a_offset,
    const float* zero,
    const union xnn_f32_minmax_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(mr != 0);
  assert(mr <= 3);
  assert(nc != 0);
  assert(kc != 0);
  assert(kc % sizeof(float) == 0);
  assert(ks != 0);
  assert(ks % (3 * sizeof(void*)) == 0);
  assert(a_offset % sizeof(float) == 0);
  assert(a != NULL);
  assert(w != NULL);
  assert(c != NULL);

  float* c0 = c;
  float* c1 = (float*) ((uintptr_t) c0 + cm_stride);
  if XNN_UNPREDICTABLE(mr < 2) {
    c1 = c0;
  }
  float* c2 = (float*) ((uintptr_t) c1 + cm_stride);
  if XNN_UNPREDICTABLE(mr <= 2) {
    c2 = c1;
  }

  do {
    float32x4_t vacc0x0 = vld1q_f32(w); w += 4;
    float32x4_t vacc0x1 = vld1q_f32(w); w += 4;
    float32x4_t vacc0x2 = vld1q_f32(w); w += 4;
    float32x4_t vacc0x3 = vld1q_f32(w); w += 4;
    float32x4_t vacc1x0 = vacc0x0;
    float32x4_t vacc1x1 = vacc0x1;
    float32x4_t vacc1x2 = vacc0x2;
    float32x4_t vacc1x3 = vacc0x3;
    float32x4_t vacc2x0 = vacc0x0;
    float32x4_t vacc2x1 = vacc0x1;
    float32x4_t vacc2x2 = vacc0x2;
    float32x4_t vacc2x3 = vacc0x3;

    size_t p = ks;
    do {
      const float* restrict a0 = a[0];
      assert(a0 != NULL);
      if XNN_UNPREDICTABLE(a0 != zero) {
        a0 = (const float*) ((uintptr_t) a0 + a_offset);
      }
      const float* restrict a1 = a[1];
      assert(a1 != NULL);
      if XNN_UNPREDICTABLE(a1 != zero) {
        a1 = (const float*) ((uintptr_t) a1 + a_offset);
      }
      const float* restrict a2 = a[2];
      assert(a2 != NULL);
      if XNN_UNPREDICTABLE(a2 != zero) {
        a2 = (const float*) ((uintptr_t) a2 + a_offset);
      }
      a += 3;

      size_t k = kc;
      for (; k >= 4 * sizeof(float); k -= 4 * sizeof(float)) {
        const float32x4_t va0 = vld1q_f32(a0); a0 += 4;
        const float32x4_t va1 = vld1q_f32(a1); a1 += 4;
        const float32x4_t va2 = vld1q_f32(a2); a2 += 4;


        const float32x4_t vb0123c0 = vld1q_f32(w); w += 4;
        const float32x4_t vb4567c0 = vld1q_f32(w); w += 4;
        const float32x4_t vb89ABc0 = vld1q_f32(w); w += 4;
        const float32x4_t vbCDEFc0 = vld1q_f32(w); w += 4;

        vacc0x0 = vfmaq_lane_f32(vacc0x0, vb0123c0, vget_low_f32(va0), 0);
        vacc1x0 = vfmaq_lane_f32(vacc1x0, vb0123c0, vget_low_f32(va1), 0);
        vacc2x0 = vfmaq_lane_f32(vacc2x0, vb0123c0, vget_low_f32(va2), 0);
        vacc0x1 = vfmaq_lane_f32(vacc0x1, vb4567c0, vget_low_f32(va0), 0);
        vacc1x1 = vfmaq_lane_f32(vacc1x1, vb4567c0, vget_low_f32(va1), 0);
        vacc2x1 = vfmaq_lane_f32(vacc2x1, vb4567c0, vget_low_f32(va2), 0);
        vacc0x2 = vfmaq_lane_f32(vacc0x2, vb89ABc0, vget_low_f32(va0), 0);
        vacc1x2 = vfmaq_lane_f32(vacc1x2, vb89ABc0, vget_low_f32(va1), 0);
        vacc2x2 = vfmaq_lane_f32(vacc2x2, vb89ABc0, vget_low_f32(va2), 0);
        vacc0x3 = vfmaq_lane_f32(vacc0x3, vbCDEFc0, vget_low_f32(va0), 0);
        vacc1x3 = vfmaq_lane_f32(vacc1x3, vbCDEFc0, vget_low_f32(va1), 0);
        vacc2x3 = vfmaq_lane_f32(vacc2x3, vbCDEFc0, vget_low_f32(va2), 0);

        const float32x4_t vb0123c1 = vld1q_f32(w); w += 4;
        const float32x4_t vb4567c1 = vld1q_f32(w); w += 4;
        const float32x4_t vb89ABc1 = vld1q_f32(w); w += 4;
        const float32x4_t vbCDEFc1 = vld1q_f32(w); w += 4;

        vacc0x0 = vfmaq_lane_f32(vacc0x0, vb0123c1, vget_low_f32(va0), 1);
        vacc1x0 = vfmaq_lane_f32(vacc1x0, vb0123c1, vget_low_f32(va1), 1);
        vacc2x0 = vfmaq_lane_f32(vacc2x0, vb0123c1, vget_low_f32(va2), 1);
        vacc0x1 = vfmaq_lane_f32(vacc0x1, vb4567c1, vget_low_f32(va0), 1);
        vacc1x1 = vfmaq_lane_f32(vacc1x1, vb4567c1, vget_low_f32(va1), 1);
        vacc2x1 = vfmaq_lane_f32(vacc2x1, vb4567c1, vget_low_f32(va2), 1);
        vacc0x2 = vfmaq_lane_f32(vacc0x2, vb89ABc1, vget_low_f32(va0), 1);
        vacc1x2 = vfmaq_lane_f32(vacc1x2, vb89ABc1, vget_low_f32(va1), 1);
        vacc2x2 = vfmaq_lane_f32(vacc2x2, vb89ABc1, vget_low_f32(va2), 1);
        vacc0x3 = vfmaq_lane_f32(vacc0x3, vbCDEFc1, vget_low_f32(va0), 1);
        vacc1x3 = vfmaq_lane_f32(vacc1x3, vbCDEFc1, vget_low_f32(va1), 1);
        vacc2x3 = vfmaq_lane_f32(vacc2x3, vbCDEFc1, vget_low_f32(va2), 1);

        const float32x4_t vb0123c2 = vld1q_f32(w); w += 4;
        const float32x4_t vb4567c2 = vld1q_f32(w); w += 4;
        const float32x4_t vb89ABc2 = vld1q_f32(w); w += 4;
        const float32x4_t vbCDEFc2 = vld1q_f32(w); w += 4;

        vacc0x0 = vfmaq_lane_f32(vacc0x0, vb0123c2, vget_high_f32(va0), 0);
        vacc1x0 = vfmaq_lane_f32(vacc1x0, vb0123c2, vget_high_f32(va1), 0);
        vacc2x0 = vfmaq_lane_f32(vacc2x0, vb0123c2, vget_high_f32(va2), 0);
        vacc0x1 = vfmaq_lane_f32(vacc0x1, vb4567c2, vget_high_f32(va0), 0);
        vacc1x1 = vfmaq_lane_f32(vacc1x1, vb4567c2, vget_high_f32(va1), 0);
        vacc2x1 = vfmaq_lane_f32(vacc2x1, vb4567c2, vget_high_f32(va2), 0);
        vacc0x2 = vfmaq_lane_f32(vacc0x2, vb89ABc2, vget_high_f32(va0), 0);
        vacc1x2 = vfmaq_lane_f32(vacc1x2, vb89ABc2, vget_high_f32(va1), 0);
        vacc2x2 = vfmaq_lane_f32(vacc2x2, vb89ABc2, vget_high_f32(va2), 0);
        vacc0x3 = vfmaq_lane_f32(vacc0x3, vbCDEFc2, vget_high_f32(va0), 0);
        vacc1x3 = vfmaq_lane_f32(vacc1x3, vbCDEFc2, vget_high_f32(va1), 0);
        vacc2x3 = vfmaq_lane_f32(vacc2x3, vbCDEFc2, vget_high_f32(va2), 0);

        const float32x4_t vb0123c3 = vld1q_f32(w); w += 4;
        const float32x4_t vb4567c3 = vld1q_f32(w); w += 4;
        const float32x4_t vb89ABc3 = vld1q_f32(w); w += 4;
        const float32x4_t vbCDEFc3 = vld1q_f32(w); w += 4;

        vacc0x0 = vfmaq_lane_f32(vacc0x0, vb0123c3, vget_high_f32(va0), 1);
        vacc1x0 = vfmaq_lane_f32(vacc1x0, vb0123c3, vget_high_f32(va1), 1);
        vacc2x0 = vfmaq_lane_f32(vacc2x0, vb0123c3, vget_high_f32(va2), 1);
        vacc0x1 = vfmaq_lane_f32(vacc0x1, vb4567c3, vget_high_f32(va0), 1);
        vacc1x1 = vfmaq_lane_f32(vacc1x1, vb4567c3, vget_high_f32(va1), 1);
        vacc2x1 = vfmaq_lane_f32(vacc2x1, vb4567c3, vget_high_f32(va2), 1);
        vacc0x2 = vfmaq_lane_f32(vacc0x2, vb89ABc3, vget_high_f32(va0), 1);
        vacc1x2 = vfmaq_lane_f32(vacc1x2, vb89ABc3, vget_high_f32(va1), 1);
        vacc2x2 = vfmaq_lane_f32(vacc2x2, vb89ABc3, vget_high_f32(va2), 1);
        vacc0x3 = vfmaq_lane_f32(vacc0x3, vbCDEFc3, vget_high_f32(va0), 1);
        vacc1x3 = vfmaq_lane_f32(vacc1x3, vbCDEFc3, vget_high_f32(va1), 1);
        vacc2x3 = vfmaq_lane_f32(vacc2x3, vbCDEFc3, vget_high_f32(va2), 1);
      }
      if XNN_UNLIKELY(k != 0) {
        do {
          const float32x4_t va0 = vld1q_dup_f32(a0); a0 += 1;
          const float32x4_t va1 = vld1q_dup_f32(a1); a1 += 1;
          const float32x4_t va2 = vld1q_dup_f32(a2); a2 += 1;

          const float32x4_t vb0123 = vld1q_f32(w); w += 4;
          const float32x4_t vb4567 = vld1q_f32(w); w += 4;
          const float32x4_t vb89AB = vld1q_f32(w); w += 4;
          const float32x4_t vbCDEF = vld1q_f32(w); w += 4;

          vacc0x0 = vfmaq_f32(vacc0x0, va0, vb0123);
          vacc1x0 = vfmaq_f32(vacc1x0, va1, vb0123);
          vacc2x0 = vfmaq_f32(vacc2x0, va2, vb0123);
          vacc0x1 = vfmaq_f32(vacc0x1, va0, vb4567);
          vacc1x1 = vfmaq_f32(vacc1x1, va1, vb4567);
          vacc2x1 = vfmaq_f32(vacc2x1, va2, vb4567);
          vacc0x2 = vfmaq_f32(vacc0x2, va0, vb89AB);
          vacc1x2 = vfmaq_f32(vacc1x2, va1, vb89AB);
          vacc2x2 = vfmaq_f32(vacc2x2, va2, vb89AB);
          vacc0x3 = vfmaq_f32(vacc0x3, va0, vbCDEF);
          vacc1x3 = vfmaq_f32(vacc1x3, va1, vbCDEF);
          vacc2x3 = vfmaq_f32(vacc2x3, va2, vbCDEF);

          k -= sizeof(float);
        } while (k != 0);
      }

      p -= 3 * sizeof(void*);
    } while (p != 0);

    const float32x4_t vmax = vdupq_n_f32(params->scalar.max);
    vacc0x0 = vminq_f32(vacc0x0, vmax);
    vacc1x0 = vminq_f32(vacc1x0, vmax);
    vacc2x0 = vminq_f32(vacc2x0, vmax);
    vacc0x1 = vminq_f32(vacc0x1, vmax);
    vacc1x1 = vminq_f32(vacc1x1, vmax);
    vacc2x1 = vminq_f32(vacc2x1, vmax);
    vacc0x2 = vminq_f32(vacc0x2, vmax);
    vacc1x2 = vminq_f32(vacc1x2, vmax);
    vacc2x2 = vminq_f32(vacc2x2, vmax);
    vacc0x3 = vminq_f32(vacc0x3, vmax);
    vacc1x3 = vminq_f32(vacc1x3, vmax);
    vacc2x3 = vminq_f32(vacc2x3, vmax);

    const float32x4_t vmin = vdupq_n_f32(params->scalar.min);
    vacc0x0 = vmaxq_f32(vacc0x0, vmin);
    vacc1x0 = vmaxq_f32(vacc1x0, vmin);
    vacc2x0 = vmaxq_f32(vacc2x0, vmin);
    vacc0x1 = vmaxq_f32(vacc0x1, vmin);
    vacc1x1 = vmaxq_f32(vacc1x1, vmin);
    vacc2x1 = vmaxq_f32(vacc2x1, vmin);
    vacc0x2 = vmaxq_f32(vacc0x2, vmin);
    vacc1x2 = vmaxq_f32(vacc1x2, vmin);
    vacc2x2 = vmaxq_f32(vacc2x2, vmin);
    vacc0x3 = vmaxq_f32(vacc0x3, vmin);
    vacc1x3 = vmaxq_f32(vacc1x3, vmin);
    vacc2x3 = vmaxq_f32(vacc2x3, vmin);

    if XNN_LIKELY(nc >= 16) {
      vst1q_f32(c2, vacc2x0);
      vst1q_f32(c2 + 4, vacc2x1);
      vst1q_f32(c2 + 8, vacc2x2);
      vst1q_f32(c2 + 12, vacc2x3);
      c2 = (float*) ((uintptr_t) c2 + cn_stride);
      vst1q_f32(c1, vacc1x0);
      vst1q_f32(c1 + 4, vacc1x1);
      vst1q_f32(c1 + 8, vacc1x2);
      vst1q_f32(c1 + 12, vacc1x3);
      c1 = (float*) ((uintptr_t) c1 + cn_stride);
      vst1q_f32(c0, vacc0x0);
      vst1q_f32(c0 + 4, vacc0x1);
      vst1q_f32(c0 + 8, vacc0x2);
      vst1q_f32(c0 + 12, vacc0x3);
      c0 = (float*) ((uintptr_t) c0 + cn_stride);

      a = (const float**restrict) ((uintptr_t) a - ks);
      nc -= 16;
    } else {
      if (nc & 8) {
        vst1q_f32(c2, vacc2x0); c2 += 4;
        vst1q_f32(c1, vacc1x0); c1 += 4;
        vst1q_f32(c0, vacc0x0); c0 += 4;
        vst1q_f32(c2, vacc2x1); c2 += 4;
        vst1q_f32(c1, vacc1x1); c1 += 4;
        vst1q_f32(c0, vacc0x1); c0 += 4;

        vacc2x0 = vacc2x2;
        vacc2x1 = vacc2x3;
        vacc1x0 = vacc1x2;
        vacc1x1 = vacc1x3;
        vacc0x0 = vacc0x2;
        vacc0x1 = vacc0x3;
      }
      if (nc & 4) {
        vst1q_f32(c2, vacc2x0); c2 += 4;
        vst1q_f32(c1, vacc1x0); c1 += 4;
        vst1q_f32(c0, vacc0x0); c0 += 4;

        vacc2x0 = vacc2x1;
        vacc2x1 = vacc2x2;
        vacc2x2 = vacc2x3;
        vacc1x0 = vacc1x1;
        vacc1x1 = vacc1x2;
        vacc1x2 = vacc1x3;
        vacc0x0 = vacc0x1;
        vacc0x1 = vacc0x2;
        vacc0x2 = vacc0x3;
      }
      float32x2_t vacc2 = vget_low_f32(vacc2x0);
      float32x2_t vacc1 = vget_low_f32(vacc1x0);
      float32x2_t vacc0 = vget_low_f32(vacc0x0);
      if (nc & 2) {
        vst1_f32(c2, vacc2); c2 += 2;
        vst1_f32(c1, vacc1); c1 += 2;
        vst1_f32(c0, vacc0); c0 += 2;

        vacc2 = vget_high_f32(vacc2x0);
        vacc1 = vget_high_f32(vacc1x0);
        vacc0 = vget_high_f32(vacc0x0);
      }
      if (nc & 1) {
        vst1_lane_f32(c2, vacc2, 0);
        vst1_lane_f32(c1, vacc1, 0);
        vst1_lane_f32(c0, vacc0, 0);
      }

      nc = 0;
    }
  } while (nc != 0);
}
