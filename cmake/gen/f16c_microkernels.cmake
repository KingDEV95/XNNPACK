# Copyright 2022 Google LLC
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
# Description: microkernel filename lists for f16c
#
# Auto-generated file. Do not edit!
#   Generator: tools/update-microkernels.py


SET(PROD_F16C_MICROKERNEL_SRCS
  src/f16-avgpool/gen/f16-avgpool-9p-minmax-f16c.c
  src/f16-f32-vcvt/gen/f16-f32-vcvt-f16c-u16.c
  src/f16-f32acc-rdsum/gen/f16-f32acc-rdsum-7p7x-f16c-c32.c
  src/f16-f32acc-rsum/gen/f16-f32acc-rsum-f16c-u32-acc4.c
  src/f16-rminmax/f16-rmax-f16c-u32.c
  src/f16-vbinary/gen/f16-vadd-f16c-u16.c
  src/f16-vbinary/gen/f16-vaddc-f16c-u16.c
  src/f16-vbinary/gen/f16-vdiv-f16c-u8.c
  src/f16-vbinary/gen/f16-vdivc-f16c-u8.c
  src/f16-vbinary/gen/f16-vmax-f16c-u16.c
  src/f16-vbinary/gen/f16-vmaxc-f16c-u16.c
  src/f16-vbinary/gen/f16-vmin-f16c-u16.c
  src/f16-vbinary/gen/f16-vminc-f16c-u16.c
  src/f16-vbinary/gen/f16-vmul-f16c-u16.c
  src/f16-vbinary/gen/f16-vmulc-f16c-u16.c
  src/f16-vbinary/gen/f16-vprelu-f16c-u16.c
  src/f16-vbinary/gen/f16-vpreluc-f16c-u16.c
  src/f16-vbinary/gen/f16-vrdivc-f16c-u8.c
  src/f16-vbinary/gen/f16-vrpreluc-f16c-u16.c
  src/f16-vbinary/gen/f16-vrsubc-f16c-u16.c
  src/f16-vbinary/gen/f16-vsqrdiff-f16c-u16.c
  src/f16-vbinary/gen/f16-vsqrdiffc-f16c-u16.c
  src/f16-vbinary/gen/f16-vsub-f16c-u16.c
  src/f16-vbinary/gen/f16-vsubc-f16c-u16.c
  src/f16-vclamp/gen/f16-vclamp-f16c-u16.c
  src/f16-vhswish/gen/f16-vhswish-f16c-u16.c
  src/f16-vlrelu/gen/f16-vlrelu-f16c-u16.c
  src/f16-vrnd/gen/f16-vrndd-f16c-u16.c
  src/f16-vrnd/gen/f16-vrndne-f16c-u16.c
  src/f16-vrnd/gen/f16-vrndu-f16c-u16.c
  src/f16-vrnd/gen/f16-vrndz-f16c-u16.c
  src/f16-vrsqrt/gen/f16-vrsqrt-f16c-rsqrt-u32.c
  src/f16-vsqrt/gen/f16-vsqrt-f16c-rsqrt-u32.c
  src/f16-vtanh/gen/f16-vtanh-f16c-expm1minus-rr1-p3h2ts-rcp-u24.c
  src/f16-vunary/gen/f16-vsqr-f16c-u16.c
  src/f32-f16-vcvt/gen/f32-f16-vcvt-f16c-u16.c)

SET(NON_PROD_F16C_MICROKERNEL_SRCS
  src/f16-f32-vcvt/gen/f16-f32-vcvt-f16c-u8.c
  src/f16-f32acc-rdsum/gen/f16-f32acc-rdsum-7p7x-f16c-c16.c
  src/f16-f32acc-rdsum/gen/f16-f32acc-rdsum-7p7x-f16c-c64.c
  src/f16-f32acc-rdsum/gen/f16-f32acc-rdsum-7p7x-f16c-c128.c
  src/f16-f32acc-rsum/gen/f16-f32acc-rsum-f16c-u8.c
  src/f16-f32acc-rsum/gen/f16-f32acc-rsum-f16c-u16-acc2.c
  src/f16-f32acc-rsum/gen/f16-f32acc-rsum-f16c-u24-acc3.c
  src/f16-f32acc-rsum/gen/f16-f32acc-rsum-f16c-u32-acc2.c
  src/f16-vbinary/gen/f16-vadd-f16c-u8.c
  src/f16-vbinary/gen/f16-vaddc-f16c-u8.c
  src/f16-vbinary/gen/f16-vdiv-f16c-u16.c
  src/f16-vbinary/gen/f16-vdivc-f16c-u16.c
  src/f16-vbinary/gen/f16-vmax-f16c-u8.c
  src/f16-vbinary/gen/f16-vmaxc-f16c-u8.c
  src/f16-vbinary/gen/f16-vmin-f16c-u8.c
  src/f16-vbinary/gen/f16-vminc-f16c-u8.c
  src/f16-vbinary/gen/f16-vmul-f16c-u8.c
  src/f16-vbinary/gen/f16-vmulc-f16c-u8.c
  src/f16-vbinary/gen/f16-vprelu-f16c-u8.c
  src/f16-vbinary/gen/f16-vpreluc-f16c-u8.c
  src/f16-vbinary/gen/f16-vrdivc-f16c-u16.c
  src/f16-vbinary/gen/f16-vrpreluc-f16c-u8.c
  src/f16-vbinary/gen/f16-vrsubc-f16c-u8.c
  src/f16-vbinary/gen/f16-vsqrdiff-f16c-u8.c
  src/f16-vbinary/gen/f16-vsqrdiffc-f16c-u8.c
  src/f16-vbinary/gen/f16-vsub-f16c-u8.c
  src/f16-vbinary/gen/f16-vsubc-f16c-u8.c
  src/f16-vclamp/gen/f16-vclamp-f16c-u8.c
  src/f16-vhswish/gen/f16-vhswish-f16c-u8.c
  src/f16-vlrelu/gen/f16-vlrelu-f16c-u8.c
  src/f16-vrnd/gen/f16-vrndd-f16c-u8.c
  src/f16-vrnd/gen/f16-vrndne-f16c-u8.c
  src/f16-vrnd/gen/f16-vrndu-f16c-u8.c
  src/f16-vrnd/gen/f16-vrndz-f16c-u8.c
  src/f16-vrsqrt/gen/f16-vrsqrt-f16c-rsqrt-u8.c
  src/f16-vrsqrt/gen/f16-vrsqrt-f16c-rsqrt-u16.c
  src/f16-vsqrt/gen/f16-vsqrt-f16c-rsqrt-u8.c
  src/f16-vsqrt/gen/f16-vsqrt-f16c-rsqrt-u16.c
  src/f16-vsqrt/gen/f16-vsqrt-f16c-sqrt-u8.c
  src/f16-vsqrt/gen/f16-vsqrt-f16c-sqrt-u16.c
  src/f16-vsqrt/gen/f16-vsqrt-f16c-sqrt-u32.c
  src/f16-vtanh/gen/f16-vtanh-f16c-expm1minus-rr1-p3h2ts-div-u8.c
  src/f16-vtanh/gen/f16-vtanh-f16c-expm1minus-rr1-p3h2ts-div-u16.c
  src/f16-vtanh/gen/f16-vtanh-f16c-expm1minus-rr1-p3h2ts-div-u24.c
  src/f16-vtanh/gen/f16-vtanh-f16c-expm1minus-rr1-p3h2ts-div-u32.c
  src/f16-vtanh/gen/f16-vtanh-f16c-expm1minus-rr1-p3h2ts-rcp-u8.c
  src/f16-vtanh/gen/f16-vtanh-f16c-expm1minus-rr1-p3h2ts-rcp-u16.c
  src/f16-vtanh/gen/f16-vtanh-f16c-expm1minus-rr1-p3h2ts-rcp-u32.c
  src/f16-vtanh/gen/f16-vtanh-f16c-polynomial-p19h9t2-u8.c
  src/f16-vtanh/gen/f16-vtanh-f16c-polynomial-p19h9t2-u16.c
  src/f16-vtanh/gen/f16-vtanh-f16c-polynomial-p19h9t2-u24.c
  src/f16-vtanh/gen/f16-vtanh-f16c-polynomial-p19h9t2-u32.c
  src/f16-vunary/gen/f16-vsqr-f16c-u8.c
  src/f32-f16-vcvt/gen/f32-f16-vcvt-f16c-u8.c)

SET(ALL_F16C_MICROKERNEL_SRCS ${PROD_F16C_MICROKERNEL_SRCS} + ${NON_PROD_F16C_MICROKERNEL_SRCS})
