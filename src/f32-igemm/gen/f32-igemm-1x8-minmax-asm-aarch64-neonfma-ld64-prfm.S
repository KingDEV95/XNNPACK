// clang-format off
// Auto-generated file. Do not edit!
//   Template: src/f32-igemm/1x8-aarch64-neonfma-ld64.S.in
//   Generator: tools/xngen
//
// Copyright 2023 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include "src/xnnpack/assembly.h"

# void xnn_f32_igemm_minmax_ukernel_1x8__asm_aarch64_neonfma_ld64_prfm(
#     size_t mr,                         (x0) - unused.  mr = 1
#     size_t nc,                         x1
#     size_t kc,                         x2 / x0
#     size_t ks,                         x3 / x9
#     const float** restrict a,           x4
#     const float* restrict w,            x5
#     float* restrict c,                  x6
#     size_t cm_stride,                  (x7) - unused
#     size_t cn_stride,                  [sp] -> x10
#     size_t a_offset,                   [sp + 8] -> x11
#     const float* zero,                 [sp + 16] -> x12
#     const xnn_f32_minmax_params params [sp + 24] -> (x7)

# d8-d15, x19-x30 need to be preserved if used. x18 is reserved by the OS.

# Register usage
# A0  x8 v0
# B   x5 v20 v21 v22 v23
# C0  x6 v16 v17 v18 v19
# Clamp  v30, v31

BEGIN_FUNCTION xnn_f32_igemm_minmax_ukernel_1x8__asm_aarch64_neonfma_ld64_prfm

        # Load cn_stride, a_offset
        LDP         x10, x11, [sp]

        # Load zero, params pointer
        LDP         x12, x7, [sp, 16]

        # Load min/max values
        LD2R        {v30.4s, v31.4s}, [x7]

0:
        # Load initial bias from w into accumulators
        LDP         q16, q17, [x5], 32
        MOVI        v18.4s, 0               // second set of C for pipelining FMLA
        PRFM        PLDL1KEEP, [x5]
        MOVI        v19.4s, 0
        PRFM        PLDL1KEEP, [x5,  64]
        PRFM        PLDL1KEEP, [x5, 128]
        PRFM        PLDL1KEEP, [x5, 192]
        PRFM        PLDL1KEEP, [x5, 256]
        PRFM        PLDL1KEEP, [x5, 320]
        PRFM        PLDL1KEEP, [x5, 384]
        PRFM        PLDL1KEEP, [x5, 448]
        PRFM        PLDL1KEEP, [x5, 512]
        PRFM        PLDL1KEEP, [x5, 576]

        MOV         x9, x3                  // p = ks

1:
        # Load next A pointer
        LDR         x8, [x4], 8

        CMP         x8, x12                 // if a0 == zero
        ADD         x8, x8, x11             // a0 += a_offset
        CSEL        x8, x12, x8, EQ         //   a0 = zero, else += a0 + a_offset

        # Is there at least 2 floats (8 bytes)
        SUBS        x0, x2, 8               // k = kc - 8
        PRFM        PLDL1KEEP, [x8,  0]     // Prefetch A
        PRFM        PLDL1KEEP, [x8, 64]

        B.LO        4f

        # Main loop - 2 floats of A (8 bytes)
2:
        LDP         q20, q21, [x5], 32
        LDR         d0, [x8], 8
        LDP         q22, q23, [x5], 32
        SUBS        x0, x0, 8
        FMLA        v16.4s, v20.4s, v0.s[0]
        FMLA        v17.4s, v21.4s, v0.s[0]
        PRFM        PLDL1KEEP, [x5, 576]        // Prefetch B
        FMLA        v18.4s, v22.4s, v0.s[1]
        FMLA        v19.4s, v23.4s, v0.s[1]
        PRFM        PLDL1KEEP, [x8, 128]        // Prefetch A0
        B.HS        2b

        # Is there a remainder?- 1 float of A (4 bytes)
        TBNZ        x0, 2, 4f

3:
        # ks loop
        SUBS        x9, x9, 8               // ks -= MR * sizeof(void*)
        B.HI        1b

        FADD        v16.4s, v16.4s, v18.4s
        FADD        v17.4s, v17.4s, v19.4s

        # Clamp
        FMAX        v16.4s, v16.4s, v30.4s
        FMAX        v17.4s, v17.4s, v30.4s
        FMIN        v16.4s, v16.4s, v31.4s
        FMIN        v17.4s, v17.4s, v31.4s

        # Store full 1 x 8
        SUBS        x1, x1, 8
        B.LO        5f

        STP         q16, q17, [x6]
        ADD         x6, x6, x10

        SUB         x4, x4, x3              // a -= ks

        # nc loop
        B.HI        0b

        RET

4:
        # Remainder- 1 float of A (4 bytes)
        LDP         q20, q21, [x5], 32
        LDR         s0, [x8], 4
        FMLA        v16.4s, v20.4s, v0.s[0]
        FMLA        v17.4s, v21.4s, v0.s[0]
        B           3b

5:
        # Store odd channels
        TBZ         x1, 2, 6f
        STR         q16, [x6], 16
        MOV         v16.16b, v17.16b

6:
        TBZ         x1, 1, 7f
        STR         d16, [x6], 8
        DUP         d16, v16.d[1]

7:
        TBZ         x1, 0, 8f
        STR         s16, [x6], 4
8:
        RET

END_FUNCTION xnn_f32_igemm_minmax_ukernel_1x8__asm_aarch64_neonfma_ld64_prfm

#ifdef __ELF__
.section ".note.GNU-stack","",%progbits
#endif
