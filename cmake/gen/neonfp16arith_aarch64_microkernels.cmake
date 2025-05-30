# Copyright 2022 Google LLC
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
# Description: microkernel filename lists for neonfp16arith_aarch64
#
# Auto-generated file. Do not edit!
#   Generator: tools/update-microkernels.py


SET(PROD_NEONFP16ARITH_AARCH64_MICROKERNEL_SRCS
  src/f16-vbinary/gen/f16-vdiv-aarch64-neonfp16arith-u8.c
  src/f16-vbinary/gen/f16-vdivc-aarch64-neonfp16arith-u8.c
  src/f16-vbinary/gen/f16-vrdivc-aarch64-neonfp16arith-u8.c
  src/f16-vsqrt/gen/f16-vsqrt-aarch64-neonfp16arith-sqrt-u8.c
  src/f16-vtanh/gen/f16-vtanh-aarch64-neonfp16arith-expm1minus-rr1-p3h2ts-div-u32.c)

SET(NON_PROD_NEONFP16ARITH_AARCH64_MICROKERNEL_SRCS
  src/f16-vbinary/gen/f16-vdiv-aarch64-neonfp16arith-u16.c
  src/f16-vbinary/gen/f16-vdivc-aarch64-neonfp16arith-u16.c
  src/f16-vbinary/gen/f16-vrdivc-aarch64-neonfp16arith-u16.c
  src/f16-vsigmoid/gen/f16-vsigmoid-aarch64-neonfp16arith-rr2-p2-div-u8.c
  src/f16-vsigmoid/gen/f16-vsigmoid-aarch64-neonfp16arith-rr2-p2-div-u16.c
  src/f16-vsigmoid/gen/f16-vsigmoid-aarch64-neonfp16arith-rr2-p2-div-u24.c
  src/f16-vsigmoid/gen/f16-vsigmoid-aarch64-neonfp16arith-rr2-p2-div-u32.c
  src/f16-vsqrt/gen/f16-vsqrt-aarch64-neonfp16arith-sqrt-u16.c
  src/f16-vsqrt/gen/f16-vsqrt-aarch64-neonfp16arith-sqrt-u32.c
  src/f16-vtanh/gen/f16-vtanh-aarch64-neonfp16arith-expm1minus-rr1-p3h2ts-div-u8.c
  src/f16-vtanh/gen/f16-vtanh-aarch64-neonfp16arith-expm1minus-rr1-p3h2ts-div-u16.c
  src/f16-vtanh/gen/f16-vtanh-aarch64-neonfp16arith-expm1minus-rr1-p3h2ts-div-u24.c)

SET(ALL_NEONFP16ARITH_AARCH64_MICROKERNEL_SRCS ${PROD_NEONFP16ARITH_AARCH64_MICROKERNEL_SRCS} + ${NON_PROD_NEONFP16ARITH_AARCH64_MICROKERNEL_SRCS})
