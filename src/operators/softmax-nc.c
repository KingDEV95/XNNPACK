// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "include/xnnpack.h"
#include "src/xnnpack/allocator.h"
#include "src/xnnpack/common.h"
#include "src/xnnpack/compute.h"
#include "src/xnnpack/config-types.h"
#include "src/xnnpack/config.h"
#include "src/xnnpack/log.h"
#include "src/xnnpack/math.h"
#include "src/xnnpack/microfnptr.h"
#include "src/xnnpack/microparams.h"
#include "src/xnnpack/operator-type.h"
#include "src/xnnpack/operator-utils.h"
#include "src/xnnpack/operator.h"
#include "src/xnnpack/params.h"
#include <pthreadpool.h>

enum xnn_status xnn_create_softmax_nc_qu8(
    float input_scale,
    uint8_t output_zero_point,
    float output_scale,
    uint32_t flags,
    xnn_operator_t* softmax_op_out)
{
  xnn_operator_t softmax_op = NULL;
  enum xnn_status status = xnn_status_uninitialized;

  if ((xnn_params.init_flags & XNN_INIT_FLAG_XNNPACK) == 0) {
    xnn_log_error("failed to create %s operator: XNNPACK is not initialized",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8));
    goto error;
  }

  status = xnn_status_invalid_parameter;

  if (input_scale <= 0.0f || !isnormal(input_scale)) {
    xnn_log_error(
      "failed to create %s operator with %.7g input scale: scale must be finite, normalized, and positive",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8), input_scale);
    goto error;
  }

  if (output_scale <= 0.0f || !isnormal(output_scale)) {
    xnn_log_error(
      "failed to create %s operator with %.7g output scale: scale must be finite, normalized, and positive",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8), output_scale);
    goto error;
  }

  status = xnn_status_unsupported_parameter;

  if (output_scale != 0x1.0p-8f) {
    xnn_log_error(
      "failed to create %s operator with %.7g output scale: only output scale of 1/256 is supported",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8), output_scale);
    goto error;
  }

  if (output_zero_point != 0) {
    xnn_log_error(
      "failed to create %s operator with %" PRIu8 " output zero point: only output zero point of 0 is supported",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8), output_zero_point);
    goto error;
  }

  status = xnn_status_out_of_memory;

  softmax_op = xnn_allocate_zero_simd_memory(sizeof(struct xnn_operator));
  if (softmax_op == NULL) {
    xnn_log_error(
      "failed to allocate %zu bytes for %s operator descriptor",
      sizeof(struct xnn_operator), xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8));
    goto error;
  }
  softmax_op->compute = xnn_allocate_zero_memory(sizeof(struct compute_parameters));
  if (softmax_op->compute == NULL) {
    xnn_log_error("failed to allocate %zu bytes for %s operator descriptor",
                  sizeof(struct compute_parameters),
                  xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8));
    goto error;
  }
  softmax_op->num_compute_invocations = 1;

  softmax_op->lookup_table = xnn_allocate_simd_memory(256 * sizeof(uint32_t));
  if (softmax_op->lookup_table == NULL) {
    xnn_log_error(
      "failed to allocate 256 bytes for %s operator lookup table",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8));
    goto error;
  }
  softmax_op->softmax.input_scale = input_scale;

  const struct xnn_lut32norm_config* lut32norm_config = xnn_init_u8_lut32norm_config();
  assert(lut32norm_config != NULL);

  const struct xnn_reduce_config* rmax_config = xnn_init_u8_rmax_config();
  assert(rmax_config != NULL);

  softmax_op->type = xnn_operator_type_softmax_nc_qu8;
  softmax_op->flags = flags;
  softmax_op->lut32norm_config = lut32norm_config;
  softmax_op->reduce_config = rmax_config;

  softmax_op->state = xnn_run_state_invalid;

  *softmax_op_out = softmax_op;
  return xnn_status_success;

error:
  xnn_delete_operator(softmax_op);
  return status;
}

enum xnn_status xnn_reshape_softmax_nc_qu8(
    xnn_operator_t softmax_op,
    size_t channels,
    size_t input_stride,
    size_t output_stride,
    size_t batch_size,
    pthreadpool_t threadpool)
{
  if (softmax_op->type != xnn_operator_type_softmax_nc_qu8) {
    xnn_log_error(
        "failed to setup operator: operator type mismatch (expected %s, got "
        "%s)",
        xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8),
        xnn_operator_type_to_string_v2(softmax_op));
    return xnn_status_invalid_parameter;
  }
  softmax_op->state = xnn_run_state_invalid;

  if (channels == 0) {
    xnn_log_error(
      "failed to create %s operator with %zu channels: number of channels must be non-zero",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8), channels);
    return xnn_status_invalid_parameter;
  }

  if (input_stride < channels) {
    xnn_log_error(
      "failed to create %s operator with input element stride of %zu: "
      "stride must be at least as large as the number of channels (%zu)",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8), input_stride, channels);
    return xnn_status_invalid_parameter;
  }

  if (output_stride < channels) {
    xnn_log_error(
      "failed to create %s operator with output element stride of %zu: "
      "stride must be at least as large as the number of channels (%zu)",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8), output_stride, channels);
    return xnn_status_invalid_parameter;
  }

  softmax_op->channels = channels;
  softmax_op->input_pixel_stride = input_stride;
  softmax_op->output_pixel_stride = output_stride;

  if ((xnn_params.init_flags & XNN_INIT_FLAG_XNNPACK) == 0) {
    xnn_log_error("failed to setup %s operator: XNNPACK is not initialized",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8));
    return xnn_status_uninitialized;
  }

  if (batch_size == 0) {
    softmax_op->state = xnn_run_state_skip;
    return xnn_status_success;
  }

  uint32_t* lookup_table = softmax_op->lookup_table;
  const double qscale = fmin(((double) UINT32_MAX) / (double) channels, 8388607.0);
  for (int32_t i = 0; i < 256; i++) {
    const double scaled_exp_xi = qscale * exp((double) (i - 255) * (double) softmax_op->softmax.input_scale);
    lookup_table[(uint32_t) i] = (uint32_t) lrint(scaled_exp_xi);
  }

  softmax_op->context.u8_softmax = (struct u8_softmax_context) {
    .n = softmax_op->channels,
    .x_stride = softmax_op->input_pixel_stride * sizeof(uint8_t),
    .t = softmax_op->lookup_table,
    .y_stride = softmax_op->output_pixel_stride * sizeof(uint8_t),
    .rmax_ukernel = (xnn_u8_rmax_ukernel_fn) softmax_op->reduce_config->ukernel,
    .lut_norm_ukernel = softmax_op->lut32norm_config->lut32norm,
  };
  softmax_op->compute[0].type = xnn_parallelization_type_1d;
  softmax_op->compute[0].task_1d = (pthreadpool_task_1d_t) xnn_compute_u8_softmax;
  softmax_op->compute[0].range[0] = batch_size;
  softmax_op->state = xnn_run_state_needs_setup;

  return xnn_status_success;
}

enum xnn_status xnn_setup_softmax_nc_qu8(
    xnn_operator_t softmax_op,
    const uint8_t* input,
    uint8_t* output)
{
  if (softmax_op->type != xnn_operator_type_softmax_nc_qu8) {
    xnn_log_error(
        "failed to setup operator: operator type mismatch (expected %s, got "
        "%s)",
        xnn_operator_type_to_string(xnn_operator_type_softmax_nc_qu8),
        xnn_operator_type_to_string_v2(softmax_op));
    return xnn_status_invalid_parameter;
  }

  switch (softmax_op->state) {
    case xnn_run_state_skip:
      return xnn_status_success;
    case xnn_run_state_invalid:
      xnn_log_error(
          "failed to setup %s operator: operator has not been reshaped yet",
          xnn_operator_type_to_string_v2(softmax_op));
      return xnn_status_invalid_state;
    case xnn_run_state_needs_setup:
      // Operator has been reshaped, but not setup, continue with setup.
    case xnn_run_state_ready:
      // Operator has been reshaped, and we are setting up with different pointers.
      break;
  }

  softmax_op->context.u8_softmax.x = input;
  softmax_op->context.u8_softmax.y = output;
  softmax_op->state = xnn_run_state_ready;

  return xnn_status_success;
}

static enum xnn_status create_softmax_nc_floating_point(
    uint32_t flags,
    const struct xnn_raddstoreexpminusmax_config* raddstoreexpminusmax_config,
    const struct xnn_reduce_config* rmax_config,
    const struct xnn_binary_elementwise_config* vmul_config,
    enum xnn_operator_type operator_type,
    xnn_operator_t* softmax_op_out)
{
  xnn_operator_t softmax_op = NULL;
  enum xnn_status status = xnn_status_uninitialized;

  if ((xnn_params.init_flags & XNN_INIT_FLAG_XNNPACK) == 0) {
    xnn_log_error("failed to create %s operator: XNNPACK is not initialized",
      xnn_operator_type_to_string(operator_type));
    goto error;
  }

  status = xnn_status_out_of_memory;

  softmax_op = xnn_allocate_zero_simd_memory(sizeof(struct xnn_operator));
  if (softmax_op == NULL) {
    xnn_log_error(
      "failed to allocate %zu bytes for %s operator descriptor",
      sizeof(struct xnn_operator), xnn_operator_type_to_string(operator_type));
    goto error;
  }
  softmax_op->compute = xnn_allocate_zero_memory(sizeof(struct compute_parameters));
  if (softmax_op->compute == NULL) {
    xnn_log_error("failed to allocate %zu bytes for %s operator descriptor",
                  sizeof(struct compute_parameters),
                  xnn_operator_type_to_string(operator_type));
    goto error;
  }
  softmax_op->num_compute_invocations = 1;

  softmax_op->type = operator_type;
  softmax_op->flags = flags;
  softmax_op->raddstoreexpminusmax_config = raddstoreexpminusmax_config;
  softmax_op->reduce_config = rmax_config;
  softmax_op->vmul_config = vmul_config;

  softmax_op->state = xnn_run_state_invalid;

  *softmax_op_out = softmax_op;
  return xnn_status_success;

error:
  xnn_delete_operator(softmax_op);
  return status;
}

enum xnn_status xnn_create_softmax_nc_f16(
    uint32_t flags,
    xnn_operator_t* softmax_op_out)
{
  const struct xnn_raddstoreexpminusmax_config* raddstoreexpminusmax_config =
    xnn_init_f16_raddstoreexpminusmax_config();
  if (raddstoreexpminusmax_config == NULL) {
    xnn_log_error("failed to create %s operator: unsupported hardware configuration",
                  xnn_operator_type_to_string(xnn_operator_type_softmax_nc_f16));
    return xnn_status_unsupported_hardware;
  }

  const struct xnn_reduce_config* rmax_config = xnn_init_f16_rmax_config();
  if (rmax_config == NULL) {
    xnn_log_error("failed to create %s operator: unsupported hardware configuration",
                  xnn_operator_type_to_string(xnn_operator_type_softmax_nc_f16));
    return xnn_status_unsupported_hardware;
  }

  const struct xnn_binary_elementwise_config* vmul_config = xnn_init_f16_vmul_config();
  if (vmul_config == NULL) {
    xnn_log_error(
      "failed to create %s operator: unsupported hardware configuration",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_f16));
    return xnn_status_unsupported_hardware;
  }

  return create_softmax_nc_floating_point(
    flags,
    raddstoreexpminusmax_config,
    rmax_config,
    vmul_config,
    xnn_operator_type_softmax_nc_f16,
    softmax_op_out);
}

enum xnn_status xnn_create_softmax_nc_f32(
    uint32_t flags,
    xnn_operator_t* softmax_op_out)
{
  const struct xnn_raddstoreexpminusmax_config* raddstoreexpminusmax_config =
    xnn_init_f32_raddstoreexpminusmax_config();
  if (raddstoreexpminusmax_config == NULL) {
    xnn_log_error(
      "failed to create %s operator: unsupported hardware configuration",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_f32));
    return xnn_status_unsupported_hardware;
  }

  const struct xnn_reduce_config* rmax_config = xnn_init_f32_rmax_config();
  if (rmax_config == NULL) {
    xnn_log_error(
      "failed to create %s operator: unsupported hardware configuration",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_f32));
    return xnn_status_unsupported_hardware;
  }

  const struct xnn_binary_elementwise_config* vmul_config = xnn_init_f32_vmul_config();
  if (vmul_config == NULL) {
    xnn_log_error(
      "failed to create %s operator: unsupported hardware configuration",
      xnn_operator_type_to_string(xnn_operator_type_softmax_nc_f32));
    return xnn_status_unsupported_hardware;
  }

  return create_softmax_nc_floating_point(
    flags,
    raddstoreexpminusmax_config,
    rmax_config,
    vmul_config,
    xnn_operator_type_softmax_nc_f32,
    softmax_op_out);
}

static enum xnn_status reshape_softmax_nc_floating_point(
    xnn_operator_t softmax_op,
    enum xnn_operator_type expected_operator_type,
    size_t channels,
    size_t input_stride,
    size_t output_stride,
    size_t batch_size,
    uint32_t log2_element_size,
    xnn_rmax_ukernel_fn rmax,
    const struct xnn_raddstoreexpminusmax_config* raddstoreexpminusmax,
    const struct xnn_binary_elementwise_config* vmul,
    xnn_compute_reciprocal_fn compute_reciprocal,
    const void* rmax_init,
    size_t rmax_init_size,
    const void* rmax_params,
    size_t rmax_params_size,
    const void* expminus_params,
    size_t expminus_params_size,
    const void* minmax_params,
    size_t minmax_params_size)
{
  if (vmul == NULL) {
    return xnn_status_unsupported_hardware;
  }
  if (softmax_op->type != expected_operator_type) {
    xnn_log_error(
        "failed to reshape operator: operator type mismatch (expected %s, got "
        "%s)",
        xnn_operator_type_to_string(expected_operator_type),
        xnn_operator_type_to_string_v2(softmax_op));
    return xnn_status_invalid_parameter;
  }
  softmax_op->state = xnn_run_state_invalid;

  if (channels == 0) {
    xnn_log_error(
      "failed to create %s operator with %zu channels: number of channels must be non-zero",
      xnn_operator_type_to_string(expected_operator_type), channels);
    return xnn_status_invalid_parameter;
  }

  if (input_stride < channels) {
    xnn_log_error(
      "failed to create %s operator with input element stride of %zu: "
      "stride must be at least as large as the number of channels (%zu)",
      xnn_operator_type_to_string(expected_operator_type), input_stride, channels);
    return xnn_status_invalid_parameter;
  }

  if (output_stride < channels) {
    xnn_log_error(
      "failed to create %s operator with output element stride of %zu: "
      "stride must be at least as large as the number of channels (%zu)",
      xnn_operator_type_to_string(expected_operator_type), output_stride, channels);
    return xnn_status_invalid_parameter;
  }

  softmax_op->channels = channels;
  softmax_op->input_pixel_stride = input_stride;
  softmax_op->output_pixel_stride = output_stride;

  if ((xnn_params.init_flags & XNN_INIT_FLAG_XNNPACK) == 0) {
    xnn_log_error("failed to reshape %s operator: XNNPACK is not initialized",
      xnn_operator_type_to_string(expected_operator_type));
    return xnn_status_uninitialized;
  }

  if (batch_size == 0) {
    softmax_op->state = xnn_run_state_skip;
    return xnn_status_success;
  }

  softmax_op->context.floating_point_softmax = (struct floating_point_softmax_context) {
    .n = softmax_op->channels << log2_element_size,
    .x_stride = softmax_op->input_pixel_stride << log2_element_size,
    .y_stride = softmax_op->output_pixel_stride << log2_element_size,
    .rmax_ukernel = rmax,
    .raddstoreexpminusmax_ukernel = raddstoreexpminusmax->ukernel,
    .compute_reciprocal = compute_reciprocal,
    .vmulc_ukernel = vmul->opc_ukernel,
  };
  if (vmul->opc_ukernel != NULL) {
    softmax_op->context.floating_point_softmax.vmulc_ukernel = vmul->opc_ukernel;
  };
  if (rmax_init_size > 0) {
    memcpy(&softmax_op->context.floating_point_softmax.rmax_init, rmax_init,
           rmax_init_size);
  }
  if (rmax_params_size > 0) {
    memcpy(&softmax_op->context.floating_point_softmax.rmax_params, rmax_params,
           rmax_params_size);
  }
  if (expminus_params_size > 0) {
    memcpy(&softmax_op->context.floating_point_softmax.expminus_params,
           expminus_params, expminus_params_size);
  }
  if (minmax_params_size > 0) {
    memcpy(&softmax_op->context.floating_point_softmax.minmax_params,
           minmax_params, minmax_params_size);
  }
  softmax_op->compute[0].type = xnn_parallelization_type_1d;
  softmax_op->compute[0].task_1d = (pthreadpool_task_1d_t) xnn_compute_floating_point_softmax;
  softmax_op->compute[0].range[0] = batch_size;
  softmax_op->state = xnn_run_state_needs_setup;

  return xnn_status_success;
}

static enum xnn_status setup_softmax_nc_floating_point(
    xnn_operator_t softmax_op,
    enum xnn_operator_type expected_operator_type,
    const void* input,
    void* output)
{
  if (softmax_op->type != expected_operator_type) {
    xnn_log_error(
        "failed to setup operator: operator type mismatch (expected %s, got "
        "%s)",
        xnn_operator_type_to_string(expected_operator_type),
        xnn_operator_type_to_string_v2(softmax_op));
    return xnn_status_invalid_parameter;
  }

  switch (softmax_op->state) {
    case xnn_run_state_skip:
      return xnn_status_success;
    case xnn_run_state_invalid:
      xnn_log_error(
          "failed to setup %s operator: operator has not been reshaped yet",
          xnn_operator_type_to_string_v2(softmax_op));
      return xnn_status_invalid_state;
    case xnn_run_state_needs_setup:
      // Operator has been reshaped, but not setup, continue with setup.
    case xnn_run_state_ready:
      // Operator has been reshaped, and we are setting up with different pointers.
      break;
  }

  softmax_op->context.floating_point_softmax.x = input;
  softmax_op->context.floating_point_softmax.y = output;
  softmax_op->state = xnn_run_state_ready;

  return xnn_status_success;
}

static void compute_reciprocal_f16(
    const xnn_float16* input,
    xnn_float16* output)
{
  *output = xnn_float16_from_float(1.0f / xnn_float16_to_float(*input));
}

enum xnn_status xnn_setup_softmax_nc_f16(
    xnn_operator_t softmax_op,
    const void* input,
    void* output)
{
  return setup_softmax_nc_floating_point(
    softmax_op, xnn_operator_type_softmax_nc_f16,
    input, output);
}

static void compute_reciprocal_f32(
    const float* input,
    float* output)
{
  *output = 1.0f / *input;
}

enum xnn_status xnn_setup_softmax_nc_f32(
    xnn_operator_t softmax_op,
    const float* input,
    float* output)
{
  return setup_softmax_nc_floating_point(
    softmax_op, xnn_operator_type_softmax_nc_f32,
    input, output);
}

enum xnn_status xnn_reshape_softmax_nc_f16(
    xnn_operator_t softmax_op,
    size_t channels,
    size_t input_stride,
    size_t output_stride,
    size_t batch_size,
    pthreadpool_t threadpool)
{
  const struct xnn_binary_elementwise_config* f16_vmul_config = softmax_op->vmul_config;

  xnn_float16 rmax_init = xnn_float16_from_float(-INFINITY);
  return reshape_softmax_nc_floating_point(
    softmax_op, xnn_operator_type_softmax_nc_f16,
    channels, input_stride, output_stride,
    batch_size,
    /*log2_element_size=*/XNN_LOG2_SIZEOF_HALF,
    softmax_op->reduce_config->ukernel,
    softmax_op->raddstoreexpminusmax_config, f16_vmul_config,
    (xnn_compute_reciprocal_fn) compute_reciprocal_f16,
    &rmax_init, sizeof(rmax_init),
    /*rmax_params=*/NULL, /*rmax_params_size=*/0,
    /*expminus_params=*/NULL, /*expminus_params_size=*/0,
    /*minmax_params=*/NULL, /*minmax_params_size=*/0);
}

enum xnn_status xnn_reshape_softmax_nc_f32(
    xnn_operator_t softmax_op,
    size_t channels,
    size_t input_stride,
    size_t output_stride,
    size_t batch_size,
    pthreadpool_t threadpool)
{
  const struct xnn_binary_elementwise_config* f32_vmul_config = softmax_op->vmul_config;

  float rmax_init = -INFINITY;
  return reshape_softmax_nc_floating_point(
    softmax_op, xnn_operator_type_softmax_nc_f32,
    channels, input_stride, output_stride,
    batch_size,
    /*log2_element_size=*/XNN_LOG2_SIZEOF_FLOAT,
    softmax_op->reduce_config->ukernel,
    softmax_op->raddstoreexpminusmax_config, f32_vmul_config,
    (xnn_compute_reciprocal_fn) compute_reciprocal_f32,
    &rmax_init, sizeof(rmax_init),
    /*rmax_params=*/NULL, /*rmax_params_size=*/0,
    /*expminus_params=*/NULL, /*expminus_params_size=*/0,
    /*minmax_params=*/NULL, /*minmax_params_size=*/0);
}
