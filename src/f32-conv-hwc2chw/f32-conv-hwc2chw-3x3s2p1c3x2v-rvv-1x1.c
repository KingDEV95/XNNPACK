// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>
#include <riscv_vector.h>

#include "src/xnnpack/conv.h"
#include "src/xnnpack/math.h"


void xnn_f32_conv_hwc2chw_ukernel_3x3s2p1c3x2v__rvv_1x1(
    size_t input_height,
    size_t input_width,
    size_t output_y_start,
    size_t output_y_end,
    const float* input,
    const float* zero,
    const float* weights,
    float* output,
    size_t input_padding_top,
    size_t output_channels,
    size_t output_height_stride,
    size_t output_channel_stride,
    const struct xnn_f32_minmax_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(input_width != 0);
  assert(output_y_end > output_y_start);
  assert(input_padding_top <= 1);
  assert(output_channels != 0);

  size_t vlmax = __riscv_vsetvlmax_e32m2();

  const size_t input_height_stride = input_width * 3 /* channels */ * sizeof(float);
  const size_t input_width_increment = round_down_po2(input_width, 2) * 3 /* channels */ * sizeof(float);
  const size_t output_width = (input_width + 1) / 2;
  const size_t output_channel_increment = output_channel_stride * vlmax - output_width * sizeof(float);

  // Adjustment for padding processed below
  const float* i0 = (const float*) ((uintptr_t) input + input_height_stride * (output_y_start * 2 - input_padding_top));
  const float* i1 = (const float*) ((uintptr_t) i0 + input_height_stride);
  const float* i2 = (const float*) ((uintptr_t) i1 + input_height_stride);
  float* output0 = (float*) ((uintptr_t) output + output_height_stride * output_y_start);

  if XNN_UNPREDICTABLE(output_y_start < input_padding_top) {
    i0 = zero;
  }

  const float voutput_max = params->scalar.max;
  const float voutput_min = params->scalar.min;

  for (size_t output_y = output_y_start; output_y < output_y_end; output_y += 1) {
    const size_t input_y2 = output_y * 2 + 2 - input_padding_top;
    if XNN_UNPREDICTABLE(input_y2 >= input_height) {
      i2 = zero;
    }

    const float* w = weights;
    size_t c = output_channels;
    float* o0 = output0;

    do {
      size_t vl = __riscv_vsetvl_e32m2(c);

      // Left edge padding
      float vi00c0 = 0.0f;
      float vi00c1 = 0.0f;
      float vi00c2 = 0.0f;
      float vi10c0 = 0.0f;
      float vi10c1 = 0.0f;
      float vi10c2 = 0.0f;
      float vi20c0 = 0.0f;
      float vi20c1 = 0.0f;
      float vi20c2 = 0.0f;

      size_t iw = input_width;
      for (; iw >= 2; iw -= 2) {
        const float* w2 = w;
        vfloat32m2_t vo0x0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;

        vfloat32m2_t vk00c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi00c0, vk00c0, vl);

        vfloat32m2_t vk10c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi10c0, vk10c0, vl);

        vfloat32m2_t vk20c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi20c0, vk20c0, vl);

        vfloat32m2_t vk00c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi00c1, vk00c1, vl);

        vfloat32m2_t vk10c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi10c1, vk10c1, vl);

        vfloat32m2_t vk20c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi20c1, vk20c1, vl);

        vfloat32m2_t vk00c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi00c2, vk00c2, vl);

        vfloat32m2_t vk10c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi10c2, vk10c2, vl);

        vfloat32m2_t vk20c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi20c2, vk20c2, vl);

        const float vi01c0 = i0[0];
        const float vi11c0 = i1[0];
        const float vi21c0 = i2[0];

        vfloat32m2_t vk01c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi01c0, vk01c0, vl);

        vfloat32m2_t vk11c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi11c0, vk11c0, vl);

        vfloat32m2_t vk21c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi21c0, vk21c0, vl);

        const float vi01c1 = i0[1];
        const float vi11c1 = i1[1];
        const float vi21c1 = i2[1];

        vfloat32m2_t vk01c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi01c1, vk01c1, vl);

        vfloat32m2_t vk11c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi11c1, vk11c1, vl);

        vfloat32m2_t vk21c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi21c1, vk21c1, vl);

        const float vi01c2 = i0[2];
        const float vi11c2 = i1[2];
        const float vi21c2 = i2[2];

        vfloat32m2_t vk01c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi01c2, vk01c2, vl);

        vfloat32m2_t vk11c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi11c2, vk11c2, vl);

        vfloat32m2_t vk21c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi21c2, vk21c2, vl);

        const float vi02c0 = i0[3];
        const float vi12c0 = i1[3];
        const float vi22c0 = i2[3];

        vfloat32m2_t vk02c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi02c0, vk02c0, vl);

        vfloat32m2_t vk12c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi12c0, vk12c0, vl);

        vfloat32m2_t vk22c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi22c0, vk22c0, vl);

        vi00c0 = vi02c0;
        vi10c0 = vi12c0;
        vi20c0 = vi22c0;

        const float vi02c1 = i0[4];
        const float vi12c1 = i1[4];
        const float vi22c1 = i2[4];

        vfloat32m2_t vk02c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi02c1, vk02c1, vl);

        vfloat32m2_t vk12c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi12c1, vk12c1, vl);

        vfloat32m2_t vk22c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi22c1, vk22c1, vl);

        vi00c1 = vi02c1;
        vi10c1 = vi12c1;
        vi20c1 = vi22c1;

        const float vi02c2 = i0[5];
        const float vi12c2 = i1[5];
        const float vi22c2 = i2[5];

        vfloat32m2_t vk02c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi02c2, vk02c2, vl);

        vfloat32m2_t vk12c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi12c2, vk12c2, vl);

        vfloat32m2_t vk22c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi22c2, vk22c2, vl);

        vi00c2 = vi02c2;
        vi10c2 = vi12c2;
        vi20c2 = vi22c2;

        vo0x0 = __riscv_vfmin_vf_f32m2(vo0x0, voutput_max, vl);
        vo0x0 = __riscv_vfmax_vf_f32m2(vo0x0, voutput_min, vl);

        __riscv_vsse32_v_f32m2(o0, output_channel_stride, vo0x0, vl);
        o0++;

        i0 += 6;
        i1 += 6;
        i2 += 6;
      }
      assert(iw < 2);
      if XNN_UNLIKELY(iw != 0) {
        const float* w2 = w;

        vfloat32m2_t vo0x0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;

        vfloat32m2_t vk00c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi00c0, vk00c0, vl);

        vfloat32m2_t vk10c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi10c0, vk10c0, vl);

        vfloat32m2_t vk20c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi20c0, vk20c0, vl);

        vfloat32m2_t vk00c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi00c1, vk00c1, vl);

        vfloat32m2_t vk10c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi10c1, vk10c1, vl);

        vfloat32m2_t vk20c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi20c1, vk20c1, vl);

        vfloat32m2_t vk00c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi00c2, vk00c2, vl);

        vfloat32m2_t vk10c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi10c2, vk10c2, vl);

        vfloat32m2_t vk20c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi20c2, vk20c2, vl);

        const float vi01c0 = i0[0];
        const float vi11c0 = i1[0];
        const float vi21c0 = i2[0];

        vfloat32m2_t vk01c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi01c0, vk01c0, vl);

        vfloat32m2_t vk11c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi11c0, vk11c0, vl);

        vfloat32m2_t vk21c0 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi21c0, vk21c0, vl);

        const float vi01c1 = i0[1];
        const float vi11c1 = i1[1];
        const float vi21c1 = i2[1];

        vfloat32m2_t vk01c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi01c1, vk01c1, vl);

        vfloat32m2_t vk11c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi11c1, vk11c1, vl);

        vfloat32m2_t vk21c1 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi21c1, vk21c1, vl);

        const float vi01c2 = i0[2];
        const float vi11c2 = i1[2];
        const float vi21c2 = i2[2];

        vfloat32m2_t vk01c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi01c2, vk01c2, vl);

        vfloat32m2_t vk11c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi11c2, vk11c2, vl);

        vfloat32m2_t vk21c2 = __riscv_vle32_v_f32m2(w2, vl);
        w2 += vlmax;
        vo0x0 = __riscv_vfmacc_vf_f32m2(vo0x0, vi21c2, vk21c2, vl);

        vo0x0 = __riscv_vfmin_vf_f32m2(vo0x0, voutput_max, vl);
        vo0x0 = __riscv_vfmax_vf_f32m2(vo0x0, voutput_min, vl);
        __riscv_vsse32_v_f32m2(o0, output_channel_stride, vo0x0, vl);

        o0++;
      }
      // Move output pointers back to the position of the first pixel in a row,
      // and forward to the next block of output channels.
      o0 = (float*) ((uintptr_t) o0 + output_channel_increment);
      // Revert input pointers to the position of the first pixel in a row
      i0 = (const float*) ((uintptr_t) i0 - input_width_increment);
      i1 = (const float*) ((uintptr_t) i1 - input_width_increment);
      i2 = (const float*) ((uintptr_t) i2 - input_width_increment);
      // Move to the block of weights for the next vlmax output channels
      w += 28 * vlmax;
      c = doz(c, vlmax);
    } while (c != 0);
    // Move output pointers forward to the next row
    output0 = (float*) ((uintptr_t) output0 + output_height_stride);
    // Move input pointers forward to the next row
    i0 = i2;
    i1 = (const float*) ((uintptr_t) i0 + input_height_stride);
    i2 = (const float*) ((uintptr_t) i1 + input_height_stride);
  }
}
