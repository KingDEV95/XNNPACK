// Copyright 2024 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <string>

#include <gtest/gtest.h>
#include "src/xnnpack/common.h"
#include "src/xnnpack/isa-checks.h"
#include "src/xnnpack/packw.h"
#include "test/next_prime.h"
#include "test/packw-microkernel-tester.h"

namespace {

struct XnnTestQS8Param {
  const char* name;
  xnn_qs8_packw_gemm_goi_ukernel_fn ukernel;
  uint64_t arch_flags;
  size_t nr, kr, sr, kblock, nr_scale, izp;
};

class XnnTestQS8 : public testing::TestWithParam<XnnTestQS8Param> {};

std::string GetTestQS8Name(
    const testing::TestParamInfo<XnnTestQS8::ParamType>& info) {
  return info.param.name;
}

#define XNN_QS8_UKERNEL(arch_flags, ukernel, nr, kr, sr, kblock, nr_scale, \
                        izp)                                               \
  {#ukernel, ukernel, arch_flags, nr, kr, sr, kblock, nr_scale, izp},

#define XNN_QS8_GIO_UKERNEL(arch_flags, ukernel, nr, kr, sr, kblock, nr_scale, \
                            izp)

const XnnTestQS8Param xnn_test_qs8_params[] = {
#include "src/qs8-packw/qs8-packw.inc"
};

#undef XNN_QS8_UKERNEL
#undef XNN_QS8_GIO_UKERNEL

}  // namespace

TEST_P(XnnTestQS8, null_bias) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  PackWMicrokernelTester()
      .nullbias(true)
      .n(GetParam().nr * GetParam().nr_scale)
      .k(GetParam().kblock)
      .nr(GetParam().nr * GetParam().nr_scale)
      .kr(GetParam().kr)
      .sr(GetParam().sr)
      .izp(GetParam().izp)
      .Test(GetParam().ukernel);
}

TEST_P(XnnTestQS8, k_eq_kblock) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  PackWMicrokernelTester()
      .n(GetParam().nr * GetParam().nr_scale)
      .k(GetParam().kblock)
      .nr(GetParam().nr * GetParam().nr_scale)
      .kr(GetParam().kr)
      .sr(GetParam().sr)
      .izp(GetParam().izp)
      .Test(GetParam().ukernel);
}

TEST_P(XnnTestQS8, k_div_kblock) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t k = GetParam().kblock; k < GetParam().kblock * 5;
       k += GetParam().kblock) {
    PackWMicrokernelTester()
        .n(GetParam().nr * GetParam().nr_scale)
        .k(k)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8, k_lt_kblock) {
  if (GetParam().kblock <= 1) {
    GTEST_SKIP();
  }
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t k = 1; k < GetParam().kblock; k++) {
    PackWMicrokernelTester()
        .n(GetParam().nr * GetParam().nr_scale)
        .k(k)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8, k_gt_kblock) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t k = GetParam().kblock + 1; k < GetParam().kblock * 5;
       k = xnnpack::NextPrime(k + 1)) {
    PackWMicrokernelTester()
        .n(GetParam().nr * GetParam().nr_scale)
        .k(k)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8, n_eq_1) {
  if (GetParam().nr <= 1 || GetParam().nr_scale != 1) {
    GTEST_SKIP();
  }
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  PackWMicrokernelTester()
      .n(1 * GetParam().nr_scale)
      .k(GetParam().kblock)
      .nr(GetParam().nr * GetParam().nr_scale)
      .kr(GetParam().kr)
      .sr(GetParam().sr)
      .izp(GetParam().izp)
      .Test(GetParam().ukernel);
}

TEST_P(XnnTestQS8, n_div_nr_null_bias) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t n = GetParam().nr; n < GetParam().nr * 5; n += GetParam().nr) {
    PackWMicrokernelTester()
        .nullbias(true)
        .n(n * GetParam().nr_scale)
        .k(GetParam().kblock)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8, n_div_nr) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t n = GetParam().nr; n < GetParam().nr * 5; n += GetParam().nr) {
    PackWMicrokernelTester()
        .n(n * GetParam().nr_scale)
        .k(GetParam().kblock)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8, n_lt_nr) {
  if (GetParam().nr <= 1 || GetParam().nr_scale != 1) {
    GTEST_SKIP();
  }
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t n = 1; n < GetParam().nr * GetParam().nr_scale; n++) {
    PackWMicrokernelTester()
        .n(n)
        .k(GetParam().kblock)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8, n_gt_nr) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t n = GetParam().nr * GetParam().nr_scale;
       n < GetParam().nr * GetParam().nr_scale * 5;
       n = xnnpack::NextPrime(n + 1)) {
    PackWMicrokernelTester()
        .n(n)
        .k(xnnpack::NextPrime(GetParam().kblock + 1))
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

INSTANTIATE_TEST_SUITE_P(qs8_packw, XnnTestQS8,
                         testing::ValuesIn(xnn_test_qs8_params),
                         GetTestQS8Name);

// Enable on all platforms when scalar available
#if XNN_ARCH_X86 || XNN_ARCH_X86_64

namespace {

struct XnnTestQS8GIOParam {
  const char* name;
  xnn_qs8_packw_gemm_gio_ukernel_fn ukernel;
  uint64_t arch_flags;
  size_t nr, kr, sr, kblock, nr_scale, izp;
};

class XnnTestQS8GIO : public testing::TestWithParam<XnnTestQS8GIOParam> {};

std::string GetTestQS8GIOName(
    const testing::TestParamInfo<XnnTestQS8GIO::ParamType>& info) {
  return info.param.name;
}

#define XNN_QS8_GIO_UKERNEL(arch_flags, ukernel, nr, kr, sr, kblock, nr_scale, \
                            izp)                                               \
  {#ukernel, ukernel, arch_flags, nr, kr, sr, kblock, nr_scale, izp},

#define XNN_QS8_UKERNEL(arch_flags, ukernel, nr, kr, sr, kblock, nr_scale, izp)

const XnnTestQS8GIOParam xnn_test_qs8_gio_params[] = {
#include "src/qs8-packw/qs8-packw.inc"
};

#undef XNN_QS8_UKERNEL
#undef XNN_QS8_GIO_UKERNEL

}  // namespace

TEST_P(XnnTestQS8GIO, null_bias) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  PackWMicrokernelTester()
      .nullbias(true)
      .n(GetParam().nr * GetParam().nr_scale)
      .k(GetParam().kblock)
      .nr(GetParam().nr * GetParam().nr_scale)
      .kr(GetParam().kr)
      .sr(GetParam().sr)
      .izp(GetParam().izp)
      .Test(GetParam().ukernel);
}

TEST_P(XnnTestQS8GIO, k_eq_kblock) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  PackWMicrokernelTester()
      .n(GetParam().nr * GetParam().nr_scale)
      .k(GetParam().kblock)
      .nr(GetParam().nr * GetParam().nr_scale)
      .kr(GetParam().kr)
      .sr(GetParam().sr)
      .izp(GetParam().izp)
      .Test(GetParam().ukernel);
}

TEST_P(XnnTestQS8GIO, k_div_kblock) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t k = GetParam().kblock; k < GetParam().kblock * 5;
       k += GetParam().kblock) {
    PackWMicrokernelTester()
        .n(GetParam().nr * GetParam().nr_scale)
        .k(k)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8GIO, k_lt_kblock) {
  if (GetParam().kblock <= 1) {
    GTEST_SKIP();
  }
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t k = 1; k < GetParam().kblock; k++) {
    PackWMicrokernelTester()
        .n(GetParam().nr * GetParam().nr_scale)
        .k(k)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8GIO, k_gt_kblock) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t k = GetParam().kblock + 1; k < GetParam().kblock * 5;
       k = xnnpack::NextPrime(k + 1)) {
    PackWMicrokernelTester()
        .n(GetParam().nr * GetParam().nr_scale)
        .k(k)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8GIO, n_eq_1) {
  if (GetParam().nr <= 1 || GetParam().nr_scale != 1) {
    GTEST_SKIP();
  }
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  PackWMicrokernelTester()
      .n(1 * GetParam().nr_scale)
      .k(GetParam().kblock)
      .nr(GetParam().nr * GetParam().nr_scale)
      .kr(GetParam().kr)
      .sr(GetParam().sr)
      .izp(GetParam().izp)
      .Test(GetParam().ukernel);
}

TEST_P(XnnTestQS8GIO, n_div_nr_null_bias) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t n = GetParam().nr; n < GetParam().nr * 5; n += GetParam().nr) {
    PackWMicrokernelTester()
        .nullbias(true)
        .n(n * GetParam().nr_scale)
        .k(GetParam().kblock)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8GIO, n_div_nr) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t n = GetParam().nr; n < GetParam().nr * 5; n += GetParam().nr) {
    PackWMicrokernelTester()
        .n(n * GetParam().nr_scale)
        .k(GetParam().kblock)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8GIO, n_lt_nr) {
  if (GetParam().nr <= 1 || GetParam().nr_scale != 1) {
    GTEST_SKIP();
  }
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t n = 1; n < GetParam().nr * GetParam().nr_scale; n++) {
    PackWMicrokernelTester()
        .n(n)
        .k(GetParam().kblock)
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

TEST_P(XnnTestQS8GIO, n_gt_nr) {
  TEST_REQUIRES_ARCH_FLAGS(GetParam().arch_flags);
  for (size_t n = GetParam().nr * GetParam().nr_scale;
       n < GetParam().nr * GetParam().nr_scale * 5;
       n = xnnpack::NextPrime(n + 1)) {
    PackWMicrokernelTester()
        .n(n)
        .k(xnnpack::NextPrime(GetParam().kblock + 1))
        .nr(GetParam().nr * GetParam().nr_scale)
        .kr(GetParam().kr)
        .sr(GetParam().sr)
        .izp(GetParam().izp)
        .Test(GetParam().ukernel);
  }
}

INSTANTIATE_TEST_SUITE_P(qs8_gio_packw, XnnTestQS8GIO,
                         testing::ValuesIn(xnn_test_qs8_gio_params),
                         GetTestQS8GIOName);

#endif  // XNN_ARCH_X86 || XNN_ARCH_X86_64
