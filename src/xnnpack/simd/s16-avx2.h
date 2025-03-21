// Copyright 2024 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//

#ifndef __XNNPACK_SRC_XNNPACK_SIMD_S16_AVX2_H_
#define __XNNPACK_SRC_XNNPACK_SIMD_S16_AVX2_H_

#include <assert.h>
#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

#include "src/xnnpack/common.h"
#include "src/xnnpack/unaligned.h"

// SIMD vector type for s16 using AVX2.
typedef __m256i xnn_simd_s16_t;
#define xnn_simd_size_s16 16
#define xnn_simd_log2_size_s16 3
#define xnn_simd_bytes_s16 (xnn_simd_size_s16 * sizeof(int16_t))

#define XNN_SIMD_CONST_S16(var, val) \
  const xnn_simd_s16_t var = _mm256_set1_epi16(val);

// Mask table used for masked load/store operations.
static const int32_t mask_table_avx_s16[30] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0, 0};
// Arithmetic operations.

static XNN_INLINE xnn_simd_s16_t xnn_min_s16(xnn_simd_s16_t a,
                                             xnn_simd_s16_t b) {
  return _mm256_min_epi16(a, b);
}

static XNN_INLINE xnn_simd_s16_t xnn_max_s16(xnn_simd_s16_t a,
                                             xnn_simd_s16_t b) {
  return _mm256_max_epi16(a, b);
}

static XNN_INLINE xnn_simd_s16_t xnn_signcomplement_s16(xnn_simd_s16_t x) {
  XNN_SIMD_CONST_S16(nonsign_mask, 0x7FFF);
  return _mm256_xor_si256(_mm256_and_si256(x, nonsign_mask),
                          _mm256_srai_epi16(x, 15));
}

// Load/store operations.

static XNN_INLINE xnn_simd_s16_t xnn_loadu_s16(const int16_t* ptr) {
  return _mm256_loadu_si256((const __m256i*)ptr);
}

static XNN_INLINE xnn_simd_s16_t xnn_load_s16(const int16_t* ptr) {
  return _mm256_load_si256((const __m256i*)ptr);
}

static XNN_INLINE void xnn_storeu_s16(int16_t* ptr, xnn_simd_s16_t v) {
  _mm256_storeu_si256((__m256i*)ptr, v);
}

static XNN_INLINE void xnn_store_s16(int16_t* ptr, xnn_simd_s16_t v) {
  _mm256_store_si256((__m256i*)ptr, v);
}

static XNN_INLINE xnn_simd_s16_t xnn_set1_s16(int16_t v) {
  return _mm256_set1_epi16(v);
}

// Tail load/store operations.

static XNN_INLINE xnn_simd_s16_t
xnn_load_tail_s16(const int16_t* input, size_t num_elements) XNN_OOB_READS {
  assert(num_elements > 0);
  assert(num_elements < xnn_simd_size_s16);
  const __m256i vmask = _mm256_loadu_si256(
      (const __m256i*)(&mask_table_avx_s16[15] - num_elements));
  return _mm256_maskload_epi32((const int32_t*) input, vmask);
}

static XNN_INLINE xnn_simd_s16_t
xnn_load_tail_safe_s16(const int16_t* input, size_t num_elements) {
  return xnn_load_tail_s16(input, num_elements);
}

static XNN_INLINE void xnn_store_tail_s16(int16_t* output, xnn_simd_s16_t v,
                                          size_t num_elements) {
  assert(num_elements > 0);
  assert(num_elements < xnn_simd_size_s16);

  __m128i v_lo = _mm256_castsi256_si128(v);
  if (num_elements & 8) {
    _mm_storeu_si128((__m128i*)output, v_lo);
    v_lo = _mm256_extractf128_si256(v, 1);
    output += 8;
  }
  if (num_elements & 4) {
    _mm_storel_epi64((__m128i*)output, v_lo);
    v_lo = _mm_unpackhi_epi64(v_lo, v_lo);
    output += 4;
  }
  if (num_elements & 2) {
    unaligned_store_u32(output, (uint32_t)_mm_cvtsi128_si32(v_lo));
    v_lo = _mm_srli_epi64(v_lo, 32);
    output += 2;
  }
  if (num_elements & 1) {
    unaligned_store_u16(output, (uint16_t)_mm_extract_epi16(v_lo, 0));
  }
}

#endif  // __XNNPACK_SRC_XNNPACK_SIMD_S16_AVX2_H_
