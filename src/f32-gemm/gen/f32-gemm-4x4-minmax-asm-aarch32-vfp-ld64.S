// clang-format off
// Auto-generated file. Do not edit!
//   Template: src/f32-gemm/4x4-aarch32-vfp-ld64.S.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include "src/xnnpack/assembly.h"

.syntax unified

// void xnn_f32_gemm_minmax_ukernel_4x4__asm_aarch32_vfp_ld64(
//     size_t mr,                            r0
//     size_t nc,                            r1
//     size_t kc,                            r2 -> r5
//     const float* a,                       r3
//     size_t a_stride,          sp + 96  -> (r11)
//     const float* w,           sp + 100 -> r9
//     float* c,                 sp + 104 -> r6
//     size_t cm_stride,         sp + 108 -> (r7)
//     size_t cn_stride,         sp + 112 -> r11
//     const xnn_f32_minmax_params* params)  sp + 116 -> (r11)

// d8-d15, r4-r11,r14(lr) need to be preserved if used. r13(sp),r15(pc) are reserved.

// Register usage
// A0   r3  s0-s1  d0
// A1  r12  s2-s3  d1
// A2  r10  s4-s5  d2
// A3   r0  s6-s7  d3
// B    r9  s12, s13, s14, s15 d6-d7
// B        s10, s11, s12, s13 d5-d6
// C0   r6 s16-s17  d8  s18-s19  d9
// C1   r4 s20-s21 d10  s22-s23 d11
// C2   r8 s24-s25 d12  s26-s27 d13
// C3   r7 s28-s29 d14  s30-s31 d15
// clamp  (r5) s8, s9 d4

BEGIN_FUNCTION xnn_f32_gemm_minmax_ukernel_4x4__asm_aarch32_vfp_ld64
        .arm
#ifndef __APPLE__
        .arch   armv6
        .fpu    vfp
#endif
        # Push 96 bytes
        PUSH    {r4, r5, r6, r7, r8, r9, r10, r11}  // 32
        VPUSH   {d8-d15}                            // +64 = 96

        LDR     r11, [sp, 96]           // Load a_stride
        LDRD    r6, r7, [sp, 104]       // Load c and cm_stride
        LDR     r5,  [sp, 116]          // Load params

        # Clamp A and C pointers
        CMP     r0, 2                   // if mr >= 2
        ADD     r12, r3, r11            //   a1 = a0 + a_stride
        ADD     r4, r6, r7              //   c1 = c0 + cm_stride
        MOVLO   r12, r3                 // a1
        MOVLO   r4, r6                  // c1

        LDR     r9, [sp, 100]           // Load w

                                        // if mr > 2
        ADD     r10, r12, r11           //   a2 = a1 + a_stride
        ADD     r8, r4, r7              //   c2 = c1 + cm_stride
        MOVLS   r10, r12                // a2
        MOVLS   r8, r4                  // c2

        VLDR    d4, [r5]                // Load min/max values

        CMP     r0, 4                   // if mr >=4
        ADD     r0, r10, r11            //   a3 = a2 + a_stride
        ADD     r7, r8, r7              //   c3 = c2 + cm_stride
        LDR     r11, [sp, 112]          // Load cn_stride
        MOVLO   r0, r10                 // a3
        MOVLO   r7, r8                  // c3


0:
        # Load initial bias from w into accumulators
        VLDM    r9!, {d8-d9}            // Bias
        SUBS    r5, r2, 8
        VMOV.F64 d10, d8
        VMOV.F64 d12, d8
        VMOV.F64 d14, d8
        VMOV.F64 d11, d9
        VMOV.F64 d13, d9
        VMOV.F64 d15, d9
        BLO     3f                      // less than 2 channels?

        # Main loop - 2 floats of A (8 bytes)
1:
        VLDM    r3!, {d0}               // A0
        VLDM    r9!, {d6-d7}            // B0
        VLDM    r12!, {d1}              // A1
        VLDM    r10!, {d2}              // A2
        VLDM    r0!, {d3}               // A3

        VMLA.F32 s16, s12, s0
        VMLA.F32 s17, s13, s0
        VMLA.F32 s20, s12, s2
        VMLA.F32 s21, s13, s2
        VMLA.F32 s24, s12, s4
        VMLA.F32 s25, s13, s4
        VMLA.F32 s28, s12, s6
        VMLA.F32 s29, s13, s6

        VMLA.F32 s18, s14, s0
        VMLA.F32 s19, s15, s0
        VMLA.F32 s22, s14, s2
        VMLA.F32 s23, s15, s2
        VLDM    r9!, {d5-d6}            // B1
        VMLA.F32 s26, s14, s4
        VMLA.F32 s27, s15, s4
        VMLA.F32 s30, s14, s6
        VMLA.F32 s31, s15, s6

        VMLA.F32 s16, s10, s1
        VMLA.F32 s17, s11, s1
        VMLA.F32 s20, s10, s3
        VMLA.F32 s21, s11, s3
        VMLA.F32 s24, s10, s5
        VMLA.F32 s25, s11, s5
        VMLA.F32 s28, s10, s7
        VMLA.F32 s29, s11, s7

        SUBS    r5, r5, 8

        VMLA.F32 s18, s12, s1
        VMLA.F32 s19, s13, s1
        VMLA.F32 s22, s12, s3
        VMLA.F32 s23, s13, s3
        VMLA.F32 s26, s12, s5
        VMLA.F32 s27, s13, s5
        VMLA.F32 s30, s12, s7
        VMLA.F32 s31, s13, s7

        BHS     1b

        # Is there a remainder?- 1 float of A (4 bytes)
        TST     r5, 4
        BNE     3f

2:
        # Clamp
        VCMPE.F32 s8, s16
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s17
        VMOVPL.F32 s16, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s18
        VMOVPL.F32 s17, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s19
        VMOVPL.F32 s18, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s20
        VMOVPL.F32 s19, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s21
        VMOVPL.F32 s20, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s22
        VMOVPL.F32 s21, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s23
        VMOVPL.F32 s22, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s24
        VMOVPL.F32 s23, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s25
        VMOVPL.F32 s24, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s26
        VMOVPL.F32 s25, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s27
        VMOVPL.F32 s26, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s28
        VMOVPL.F32 s27, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s29
        VMOVPL.F32 s28, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s30
        VMOVPL.F32 s29, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s8, s31
        VMOVPL.F32 s30, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s16
        VMOVPL.F32 s31, s8
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s17
        VMOVMI.F32 s16, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s18
        VMOVMI.F32 s17, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s19
        VMOVMI.F32 s18, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s20
        VMOVMI.F32 s19, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s21
        VMOVMI.F32 s20, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s22
        VMOVMI.F32 s21, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s23
        VMOVMI.F32 s22, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s24
        VMOVMI.F32 s23, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s25
        VMOVMI.F32 s24, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s26
        VMOVMI.F32 s25, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s27
        VMOVMI.F32 s26, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s28
        VMOVMI.F32 s27, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s29
        VMOVMI.F32 s28, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s30
        VMOVMI.F32 s29, s9
        VMRS    APSR_nzcv, FPSCR
        VCMPE.F32 s9, s31
        VMOVMI.F32 s30, s9
        VMRS    APSR_nzcv, FPSCR
        VMOVMI.F32 s31, s9

        SUBS    r1, r1, 4
        BLO     4f

        # Store full 4 x 4
        VSTM    r6, {d8-d9}
        SUB     r0, r0, r2
        ADD     r6, r11
        VSTM    r4, {d10-d11}
        SUB     r10, r10, r2
        ADD     r4, r11
        VSTM    r8, {d12-d13}
        SUB     r12, r12, r2
        ADD     r8, r11
        VSTM    r7, {d14-d15}
        SUB     r3, r3, r2
        ADD     r7, r11
        BHI     0b

        VPOP    {d8-d15}
        POP     {r4, r5, r6, r7, r8, r9, r10, r11}
        BX      lr

3:
        # Remainder- 1 float of A (4 bytes)
        VLDM    r3!, {s0}               // A0
        VLDM    r9!, {d6-d7}            // B
        VLDM    r12!, {s1}              // A1
        VLDM    r10!, {s2}              // A2
        VLDM    r0!, {s3}               // A3

        VMLA.F32 s16, s12, s0
        VMLA.F32 s17, s13, s0
        VMLA.F32 s18, s14, s0
        VMLA.F32 s19, s15, s0

        VMLA.F32 s20, s12, s1
        VMLA.F32 s21, s13, s1
        VMLA.F32 s22, s14, s1
        VMLA.F32 s23, s15, s1

        VMLA.F32 s24, s12, s2
        VMLA.F32 s25, s13, s2
        VMLA.F32 s26, s14, s2
        VMLA.F32 s27, s15, s2

        VMLA.F32 s28, s12, s3
        VMLA.F32 s29, s13, s3
        VMLA.F32 s30, s14, s3
        VMLA.F32 s31, s15, s3

        B       2b

        # Store odd width
4:
        TST     r1, 2
        BEQ     5f
        VSTM    r6!, {d8}
        VMOV.F32 s16, s18
        VSTM    r4!, {d10}
        VMOV.F32 s20, s22
        VSTM    r8!, {d12}
        VMOV.F32 s24, s26
        VSTM    r7!, {d14}
        VMOV.F32 s28, s30

5:
        TST     r1, 1
        BEQ     6f
        VSTR    s16, [r6]
        VSTR    s20, [r4]
        VSTR    s24, [r8]
        VSTR    s28, [r7]

6:
        VPOP    {d8-d15}
        POP     {r4, r5, r6, r7, r8, r9, r10, r11}
        BX      lr

END_FUNCTION xnn_f32_gemm_minmax_ukernel_4x4__asm_aarch32_vfp_ld64

#ifdef __ELF__
.section ".note.GNU-stack","",%progbits
#endif
