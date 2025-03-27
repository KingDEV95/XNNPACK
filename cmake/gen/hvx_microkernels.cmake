# Copyright 2022 Google LLC
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
# Description: microkernel filename lists for hvx
#
# Auto-generated file. Do not edit!
#   Generator: tools/update-microkernels.py


SET(PROD_HVX_MICROKERNEL_SRCS
  src/f32-gemm/gen/f32-gemm-1x32-minmax-hvx-broadcast.c
  src/f32-gemm/gen/f32-gemm-8x32-minmax-hvx-broadcast.c
  src/f32-igemm/gen/f32-igemm-1x32-minmax-hvx-broadcast.c
  src/f32-igemm/gen/f32-igemm-8x32-minmax-hvx-broadcast.c
  src/f32-rdminmax/gen/f32-rdmax-2p2x-hvx-c32.c
  src/f32-rdminmax/gen/f32-rdmin-2p2x-hvx-c32.c
  src/x32-packw/gen/x32-packw-gio-hvx-u2.c)

SET(NON_PROD_HVX_MICROKERNEL_SRCS
  src/f32-gemm/gen/f32-gemm-1x64-minmax-hvx-broadcast.c
  src/f32-gemm/gen/f32-gemm-1x128-minmax-hvx-broadcast.c
  src/f32-gemm/gen/f32-gemm-2x128-minmax-hvx-broadcast.c
  src/f32-gemm/gen/f32-gemm-4x64-minmax-hvx-broadcast.c
  src/f32-gemm/gen/f32-gemm-7x32-minmax-hvx-broadcast.c
  src/f32-gemm/gen/f32-gemm-7x64-minmax-hvx-broadcast.c
  src/f32-gemm/gen/f32-gemm-16x32-minmax-hvx-broadcast.c
  src/f32-igemm/gen/f32-igemm-1x64-minmax-hvx-broadcast.c
  src/f32-igemm/gen/f32-igemm-1x128-minmax-hvx-broadcast.c
  src/f32-igemm/gen/f32-igemm-2x128-minmax-hvx-broadcast.c
  src/f32-igemm/gen/f32-igemm-4x64-minmax-hvx-broadcast.c
  src/f32-igemm/gen/f32-igemm-7x64-minmax-hvx-broadcast.c
  src/f32-igemm/gen/f32-igemm-16x32-minmax-hvx-broadcast.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-hvx-u32.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-hvx-u64.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-hvx-u96.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-hvx-u128.c
  src/f32-qs8-vcvt/gen/f32-qs8-vcvt-hvx-u256.c
  src/f32-raddstoreexpminusmax/gen/f32-raddstoreexpminusmax-hvx-rr2-p5-u32.c
  src/f32-raddstoreexpminusmax/gen/f32-raddstoreexpminusmax-hvx-rr2-p5-u64-acc2.c
  src/f32-raddstoreexpminusmax/gen/f32-raddstoreexpminusmax-hvx-rr2-p5-u128-acc2.c
  src/f32-raddstoreexpminusmax/gen/f32-raddstoreexpminusmax-hvx-rr2-p5-u128-acc4.c
  src/f32-rsum/gen/f32-rsum-hvx-u32.c
  src/f32-rsum/gen/f32-rsum-hvx-u64-acc2.c
  src/f32-rsum/gen/f32-rsum-hvx-u96-acc3.c
  src/f32-rsum/gen/f32-rsum-hvx-u128-acc2.c
  src/f32-rsum/gen/f32-rsum-hvx-u128-acc4.c
  src/f32-spmm/gen/f32-spmm-32x1-minmax-hvx-pipelined-x2.c
  src/f32-spmm/gen/f32-spmm-32x1-minmax-hvx-pipelined-x4.c
  src/f32-spmm/gen/f32-spmm-32x1-minmax-hvx-pipelined.c
  src/f32-spmm/gen/f32-spmm-32x1-minmax-hvx-x2.c
  src/f32-spmm/gen/f32-spmm-32x1-minmax-hvx-x4.c
  src/f32-spmm/gen/f32-spmm-32x1-minmax-hvx.c
  src/f32-spmm/gen/f32-spmm-64x1-minmax-hvx-pipelined-x2.c
  src/f32-spmm/gen/f32-spmm-64x1-minmax-hvx-pipelined-x4.c
  src/f32-spmm/gen/f32-spmm-64x1-minmax-hvx-pipelined.c
  src/f32-spmm/gen/f32-spmm-64x1-minmax-hvx-x2.c
  src/f32-spmm/gen/f32-spmm-64x1-minmax-hvx-x4.c
  src/f32-spmm/gen/f32-spmm-64x1-minmax-hvx.c
  src/f32-spmm/gen/f32-spmm-128x1-minmax-hvx-pipelined-x2.c
  src/f32-spmm/gen/f32-spmm-128x1-minmax-hvx-pipelined-x4.c
  src/f32-spmm/gen/f32-spmm-128x1-minmax-hvx-pipelined.c
  src/f32-spmm/gen/f32-spmm-128x1-minmax-hvx-x2.c
  src/f32-spmm/gen/f32-spmm-128x1-minmax-hvx-x4.c
  src/f32-spmm/gen/f32-spmm-128x1-minmax-hvx.c
  src/f32-vapproxgelu/gen/f32-vapproxgelu-hvx-rational-12-10-div.c
  src/f32-vbinary/gen/f32-vadd-hvx-u32.c
  src/f32-vbinary/gen/f32-vadd-hvx-u64.c
  src/f32-vbinary/gen/f32-vadd-hvx-u128.c
  src/f32-vbinary/gen/f32-vaddc-hvx-u32.c
  src/f32-vbinary/gen/f32-vaddc-hvx-u64.c
  src/f32-vbinary/gen/f32-vaddc-hvx-u128.c
  src/f32-vbinary/gen/f32-vmax-hvx-u32.c
  src/f32-vbinary/gen/f32-vmax-hvx-u64.c
  src/f32-vbinary/gen/f32-vmax-hvx-u128.c
  src/f32-vbinary/gen/f32-vmaxc-hvx-u32.c
  src/f32-vbinary/gen/f32-vmaxc-hvx-u64.c
  src/f32-vbinary/gen/f32-vmaxc-hvx-u128.c
  src/f32-vbinary/gen/f32-vmin-hvx-u32.c
  src/f32-vbinary/gen/f32-vmin-hvx-u64.c
  src/f32-vbinary/gen/f32-vmin-hvx-u128.c
  src/f32-vbinary/gen/f32-vminc-hvx-u32.c
  src/f32-vbinary/gen/f32-vminc-hvx-u64.c
  src/f32-vbinary/gen/f32-vminc-hvx-u128.c
  src/f32-vbinary/gen/f32-vmul-hvx-u32.c
  src/f32-vbinary/gen/f32-vmul-hvx-u64.c
  src/f32-vbinary/gen/f32-vmul-hvx-u128.c
  src/f32-vbinary/gen/f32-vmulc-hvx-u32.c
  src/f32-vbinary/gen/f32-vmulc-hvx-u64.c
  src/f32-vbinary/gen/f32-vmulc-hvx-u128.c
  src/f32-vbinary/gen/f32-vrsubc-hvx-u32.c
  src/f32-vbinary/gen/f32-vrsubc-hvx-u64.c
  src/f32-vbinary/gen/f32-vrsubc-hvx-u128.c
  src/f32-vbinary/gen/f32-vsqrdiff-hvx-u32.c
  src/f32-vbinary/gen/f32-vsqrdiff-hvx-u64.c
  src/f32-vbinary/gen/f32-vsqrdiff-hvx-u128.c
  src/f32-vbinary/gen/f32-vsqrdiffc-hvx-u32.c
  src/f32-vbinary/gen/f32-vsqrdiffc-hvx-u64.c
  src/f32-vbinary/gen/f32-vsqrdiffc-hvx-u128.c
  src/f32-vbinary/gen/f32-vsub-hvx-u32.c
  src/f32-vbinary/gen/f32-vsub-hvx-u64.c
  src/f32-vbinary/gen/f32-vsub-hvx-u128.c
  src/f32-vbinary/gen/f32-vsubc-hvx-u32.c
  src/f32-vbinary/gen/f32-vsubc-hvx-u64.c
  src/f32-vbinary/gen/f32-vsubc-hvx-u128.c
  src/f32-vgelu/gen/f32-vgelu-hvx-rational-12-10-div.c
  src/f32-vunary/gen/f32-vabs-hvx.c
  src/f32-vunary/gen/f32-vneg-hvx.c
  src/f32-vunary/gen/f32-vsqr-hvx.c
  src/qs8-vadd/gen/qs8-vadd-minmax-hvx-u32.c
  src/qs8-vadd/gen/qs8-vadd-minmax-hvx-u64.c
  src/qs8-vadd/gen/qs8-vadd-minmax-hvx-u96.c
  src/qs8-vadd/gen/qs8-vadd-minmax-hvx-u128.c)

SET(ALL_HVX_MICROKERNEL_SRCS ${PROD_HVX_MICROKERNEL_SRCS} + ${NON_PROD_HVX_MICROKERNEL_SRCS})
