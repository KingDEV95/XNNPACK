// clang-format off
// Auto-generated file. Do not edit!
//   Template: src/qs8-dwconv/unipass-scalar.c.in
//   Generator: tools/xngen
//
// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "src/xnnpack/common.h"
#include "src/xnnpack/dwconv.h"
#include "src/xnnpack/math.h"
#include "src/xnnpack/microparams.h"
#include "src/xnnpack/unaligned.h"

void xnn_qs8_qc8w_dwconv_minmax_fp32_ukernel_9p1c__scalar_fmagic(
    size_t channels,
    size_t output_width,
    const int8_t** input,
    const void* weights,
    int8_t* output,
    intptr_t input_stride,
    size_t output_increment,
    size_t input_offset,
    size_t input_pixel_stride,
    const int8_t* zero,
    const union xnn_qs8_qc8w_conv_minmax_params* restrict params)
{
  assert(channels != 0);
  assert(output_width != 0);

  const float voutput_min_less_zero_point = (int32_t) params->fp32_scalar.output_min - (int32_t) params->fp32_scalar.output_zero_point;
  const float voutput_max_less_zero_point = (int32_t) params->fp32_scalar.output_max - (int32_t) params->fp32_scalar.output_zero_point;
  const float vmagic_bias = 12582912.0f;
  const int32_t vmagic_bias_less_output_zero_point = INT32_C(0x4B400000) - (int32_t) params->fp32_scalar.output_zero_point;
  do {
    const int8_t* i0 = input[0];
    assert(i0 != NULL);
    if XNN_UNPREDICTABLE(i0 != zero) {
      i0 = (const int8_t*) ((uintptr_t) i0 + input_offset);
    }
    const int8_t* i1 = input[1];
    assert(i1 != NULL);
    if XNN_UNPREDICTABLE(i1 != zero) {
      i1 = (const int8_t*) ((uintptr_t) i1 + input_offset);
    }
    const int8_t* i2 = input[2];
    assert(i2 != NULL);
    if XNN_UNPREDICTABLE(i2 != zero) {
      i2 = (const int8_t*) ((uintptr_t) i2 + input_offset);
    }
    const int8_t* i3 = input[3];
    assert(i3 != NULL);
    if XNN_UNPREDICTABLE(i3 != zero) {
      i3 = (const int8_t*) ((uintptr_t) i3 + input_offset);
    }
    const int8_t* i4 = input[4];
    assert(i4 != NULL);
    if XNN_UNPREDICTABLE(i4 != zero) {
      i4 = (const int8_t*) ((uintptr_t) i4 + input_offset);
    }
    const int8_t* i5 = input[5];
    assert(i5 != NULL);
    if XNN_UNPREDICTABLE(i5 != zero) {
      i5 = (const int8_t*) ((uintptr_t) i5 + input_offset);
    }
    const int8_t* i6 = input[6];
    assert(i6 != NULL);
    if XNN_UNPREDICTABLE(i6 != zero) {
      i6 = (const int8_t*) ((uintptr_t) i6 + input_offset);
    }
    const int8_t* i7 = input[7];
    assert(i7 != NULL);
    if XNN_UNPREDICTABLE(i7 != zero) {
      i7 = (const int8_t*) ((uintptr_t) i7 + input_offset);
    }
    const int8_t* i8 = input[8];
    assert(i8 != NULL);
    if XNN_UNPREDICTABLE(i8 != zero) {
      i8 = (const int8_t*) ((uintptr_t) i8 + input_offset);
    }
    input = (const int8_t**) ((uintptr_t) input + input_stride);

    size_t c = channels;
    const void* w = weights;
    do {
      int32_t vacc = unaligned_load_s32(w);

      const int32_t vi0 = (int32_t) *i0++;
      const int32_t vk0 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[0];
      vacc += vi0 * vk0;
      const int32_t vi1 = (int32_t) *i1++;
      const int32_t vk1 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[1];
      vacc += vi1 * vk1;
      const int32_t vi2 = (int32_t) *i2++;
      const int32_t vk2 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[2];
      vacc += vi2 * vk2;
      const int32_t vi3 = (int32_t) *i3++;
      const int32_t vk3 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[3];
      vacc += vi3 * vk3;
      const int32_t vi4 = (int32_t) *i4++;
      const int32_t vk4 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[4];
      vacc += vi4 * vk4;
      const int32_t vi5 = (int32_t) *i5++;
      const int32_t vk5 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[5];
      vacc += vi5 * vk5;
      const int32_t vi6 = (int32_t) *i6++;
      const int32_t vk6 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[6];
      vacc += vi6 * vk6;
      const int32_t vi7 = (int32_t) *i7++;
      const int32_t vk7 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[7];
      vacc += vi7 * vk7;
      const int32_t vi8 = (int32_t) *i8++;
      const int32_t vk8 = ((const int8_t*) ((uintptr_t) w + sizeof(int32_t)))[8];
      vacc += vi8 * vk8;

      w = (const void*) ((uintptr_t) w + sizeof(int32_t) + 9 * sizeof(int8_t));

      const float vscale = unaligned_load_f32(w);
      w = (const void*) ((const float*) w + 1);
      float vfpacc = (float) vacc * vscale;

      vfpacc = math_max_f32(vfpacc, voutput_min_less_zero_point);
      vfpacc = math_min_f32(vfpacc, voutput_max_less_zero_point);
      vfpacc += vmagic_bias;
      int32_t vout = (int32_t) float_as_uint32(vfpacc) - vmagic_bias_less_output_zero_point;

      *output++ = (int8_t) vout;
    } while (--c != 0);

    input_offset += input_pixel_stride;
    output = (int8_t*) ((uintptr_t) output + output_increment);
  } while (--output_width != 0);
}
