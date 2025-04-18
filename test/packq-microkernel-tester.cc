// Copyright 2024 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include "test/packq-microkernel-tester.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <numeric>
#include <vector>

#include <gtest/gtest.h>
#include "include/xnnpack.h"
#include "src/xnnpack/buffer.h"
#include "src/xnnpack/math.h"
#include "src/xnnpack/microfnptr.h"
#include "src/xnnpack/packq.h"

namespace xnnpack {

void PackQMicrokernelTester::Test(xnn_x8_packq_f32qp8_ukernel_fn packq) const {
  // Allocate the input and output data.
  xnnpack::Buffer<float> input(m() * k(), xnnpack::XnnExtraBytes);
  const size_t packed_size =
      xnn_x8_packq_f32qp8_packed_size(m(), k(), mr(), kr(), sr());
  xnnpack::Buffer<int8_t, XNN_ALLOCATION_ALIGNMENT> packed_w(packed_size);
  xnnpack::Buffer<int8_t, XNN_ALLOCATION_ALIGNMENT> packed_w_ref(packed_size);

  // Populate the input and output data.
  std::iota(input.begin(), input.end(), 0.0f);
  // TODO(b/372820266): Remove these fill calls that hide uninitialized memory
  // bugs.
  std::fill(packed_w.begin(), packed_w.end(), INT8_C(0x12));
  std::fill(packed_w_ref.begin(), packed_w_ref.end(), INT8_C(0x7B));

  // Compute reference results.
  xnn_x8_packq_f32qp8_ukernel__scalar_u1(
      m(), k(), mr(), kr(), sr(), /*m_idx_start=*/0, input.data(),
      /*lhs_stride=*/k() * sizeof(float), packed_w_ref.data());

  // Call optimized micro-kernel.
  packq(m(), k(), mr(), kr(), sr(), /*m_idx_start=*/0, input.data(),
        /*lhs_stride=*/k() * sizeof(float), packed_w.data());

  // Verify results.
  for (size_t i = 0; i < packed_size; i++) {
    if (packed_w_ref[i] != INT8_C(0x7B)) {  // Allow pad to differ
      ASSERT_EQ((int32_t)packed_w[i], (int32_t)packed_w_ref[i])
          << "at n " << i << " of " << packed_size << ", m=" << m()
          << ", k=" << k() << ", mr=" << mr() << ", kr=" << kr()
          << ", sr=" << sr();
    }
  }
}

};  // namespace xnnpack
