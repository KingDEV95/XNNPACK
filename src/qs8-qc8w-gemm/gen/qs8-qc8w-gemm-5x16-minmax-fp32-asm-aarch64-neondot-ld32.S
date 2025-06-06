// Copyright 2025 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include "src/xnnpack/assembly.h"

BEGIN_FUNCTION xnn_qs8_qc8w_gemm_minmax_fp32_ukernel_5x16c4__asm_aarch64_neondot_ld32_2

      # Free up GP registers.
      sub sp, sp, 256
      stp x27, x28, [sp, 224]
      stp x25, x26, [sp, 192]
      stp x23, x24, [sp, 160]
      stp x21, x22, [sp, 128]
      stp x19, x20, [sp, 96]

      # Preserve callee saved q8-q15 registers.
      stp d8, d9, [sp, 64]
      stp d10, d11, [sp, 48]
      stp d12, d13, [sp, 32]
      stp d14, d15, [sp, 16]

      # Load params.
      ldr x13, [sp, 264]

      # Load min/max values.
      add x13, x13, 2
      ld2r {v0.16b, v1.16b}, [x13]
      sub x13, x13, 2
      ldr x24, [sp, 272]
      # Round kc up to channels.
      add x2, x2, #3
      and x2, x2, #0xFFFFFFFFFFFFFFFC

      # Setup and alias a & c pointers.
      add x9, x3, x4
      add x10, x9, x4
      add x11, x10, x4
      add x12, x11, x4
      add x14, x6, x7
      add x15, x14, x7
      add x19, x15, x7
      add x23, x19, x7

      cmp x0, 2
      csel  x9, x3, x9, LO
      csel  x14, x6, x14, LO
      csel  x10, x9, x10, LS
      csel  x15, x14, x15, LS

      cmp x0, 4
      csel  x11, x10, x11, LO
      csel  x19, x15, x19, LO
      csel  x12, x11, x12, LS
      csel  x23, x19, x23, LS

.Louter_loop:
      # Initialize k counter.
      mov x20, x2

      # Initialize accumulators with the biases.
      ldp q12, q13, [x5, 0]
      ldp q14, q15, [x5, 32]
      mov v16.16b, v12.16b
      mov v20.16b, v12.16b
      mov v24.16b, v12.16b
      mov v28.16b, v12.16b
      mov v17.16b, v13.16b
      mov v21.16b, v13.16b
      mov v25.16b, v13.16b
      mov v29.16b, v13.16b
      mov v18.16b, v14.16b
      mov v22.16b, v14.16b
      mov v26.16b, v14.16b
      mov v30.16b, v14.16b
      mov v19.16b, v15.16b
      mov v23.16b, v15.16b
      mov v27.16b, v15.16b
      mov v31.16b, v15.16b
      add x5, x5, 64

.Linner_loop:
      ldr s2, [x3], 4
      ldr s3, [x9], 4
      ldr s4, [x10], 4
      ldr s5, [x11], 4
      ldr s11, [x12], 4
      ldp q6, q7, [x5], 32
      ldp q8, q9, [x5], 32
      sdot  v12.4s, v6.16b, v2.4b[0]
      sdot  v16.4s, v6.16b, v3.4b[0]
      sdot  v20.4s, v6.16b, v4.4b[0]
      sdot  v24.4s, v6.16b, v5.4b[0]
      sdot  v28.4s, v6.16b, v11.4b[0]
      sdot  v13.4s, v7.16b, v2.4b[0]
      sdot  v17.4s, v7.16b, v3.4b[0]
      sdot  v21.4s, v7.16b, v4.4b[0]
      sdot  v25.4s, v7.16b, v5.4b[0]
      sdot  v29.4s, v7.16b, v11.4b[0]
      sdot  v14.4s, v8.16b, v2.4b[0]
      sdot  v18.4s, v8.16b, v3.4b[0]
      sdot  v22.4s, v8.16b, v4.4b[0]
      sdot  v26.4s, v8.16b, v5.4b[0]
      sdot  v30.4s, v8.16b, v11.4b[0]
      sdot  v15.4s, v9.16b, v2.4b[0]
      sdot  v19.4s, v9.16b, v3.4b[0]
      sdot  v23.4s, v9.16b, v4.4b[0]
      sdot  v27.4s, v9.16b, v5.4b[0]
      sdot  v31.4s, v9.16b, v11.4b[0]
      subs x20, x20, 4
      bne .Linner_loop


.Linner_loop_end:
      # Convert from int32 to float.
      scvtf v12.4s, v12.4s
      scvtf v13.4s, v13.4s
      scvtf v14.4s, v14.4s
      scvtf v15.4s, v15.4s
      scvtf v16.4s, v16.4s
      scvtf v17.4s, v17.4s
      scvtf v18.4s, v18.4s
      scvtf v19.4s, v19.4s
      scvtf v20.4s, v20.4s
      scvtf v21.4s, v21.4s
      scvtf v22.4s, v22.4s
      scvtf v23.4s, v23.4s
      scvtf v24.4s, v24.4s
      scvtf v25.4s, v25.4s
      scvtf v26.4s, v26.4s
      scvtf v27.4s, v27.4s
      scvtf v28.4s, v28.4s
      scvtf v29.4s, v29.4s
      scvtf v30.4s, v30.4s
      scvtf v31.4s, v31.4s
      # Load weights scale.
      ldp q2, q3, [x5, 0]
      ldp q4, q5, [x5, 32]
      add x5, x5, 64
      # Multiply by weight's scale.
      fmul v12.4s, v12.4s, v2.4s
      fmul v16.4s, v16.4s, v2.4s
      fmul v20.4s, v20.4s, v2.4s
      fmul v24.4s, v24.4s, v2.4s
      fmul v28.4s, v28.4s, v2.4s
      fmul v13.4s, v13.4s, v3.4s
      fmul v17.4s, v17.4s, v3.4s
      fmul v21.4s, v21.4s, v3.4s
      fmul v25.4s, v25.4s, v3.4s
      fmul v29.4s, v29.4s, v3.4s
      fmul v14.4s, v14.4s, v4.4s
      fmul v18.4s, v18.4s, v4.4s
      fmul v22.4s, v22.4s, v4.4s
      fmul v26.4s, v26.4s, v4.4s
      fmul v30.4s, v30.4s, v4.4s
      fmul v15.4s, v15.4s, v5.4s
      fmul v19.4s, v19.4s, v5.4s
      fmul v23.4s, v23.4s, v5.4s
      fmul v27.4s, v27.4s, v5.4s
      fmul v31.4s, v31.4s, v5.4s
      # Reconvert to int32.
      fcvtns v12.4s, v12.4s
      fcvtns v13.4s, v13.4s
      fcvtns v14.4s, v14.4s
      fcvtns v15.4s, v15.4s
      fcvtns v16.4s, v16.4s
      fcvtns v17.4s, v17.4s
      fcvtns v18.4s, v18.4s
      fcvtns v19.4s, v19.4s
      fcvtns v20.4s, v20.4s
      fcvtns v21.4s, v21.4s
      fcvtns v22.4s, v22.4s
      fcvtns v23.4s, v23.4s
      fcvtns v24.4s, v24.4s
      fcvtns v25.4s, v25.4s
      fcvtns v26.4s, v26.4s
      fcvtns v27.4s, v27.4s
      fcvtns v28.4s, v28.4s
      fcvtns v29.4s, v29.4s
      fcvtns v30.4s, v30.4s
      fcvtns v31.4s, v31.4s
      # Convert to int16.
      sqxtn v12.4h, v12.4s
      sqxtn v16.4h, v16.4s
      sqxtn v20.4h, v20.4s
      sqxtn v24.4h, v24.4s
      sqxtn v28.4h, v28.4s
      sqxtn v14.4h, v14.4s
      sqxtn v18.4h, v18.4s
      sqxtn v22.4h, v22.4s
      sqxtn v26.4h, v26.4s
      sqxtn v30.4h, v30.4s
      sqxtn2 v12.8h, v13.4s
      sqxtn2 v16.8h, v17.4s
      sqxtn2 v20.8h, v21.4s
      sqxtn2 v24.8h, v25.4s
      sqxtn2 v28.8h, v29.4s
      sqxtn2 v14.8h, v15.4s
      sqxtn2 v18.8h, v19.4s
      sqxtn2 v22.8h, v23.4s
      sqxtn2 v26.8h, v27.4s
      sqxtn2 v30.8h, v31.4s
      ld1r {v9.8h}, [x13]
      # Add output zero point.
      sqadd v12.8h, v12.8h, v9.8h
      sqadd v16.8h, v16.8h, v9.8h
      sqadd v20.8h, v20.8h, v9.8h
      sqadd v24.8h, v24.8h, v9.8h
      sqadd v28.8h, v28.8h, v9.8h
      sqadd v14.8h, v14.8h, v9.8h
      sqadd v18.8h, v18.8h, v9.8h
      sqadd v22.8h, v22.8h, v9.8h
      sqadd v26.8h, v26.8h, v9.8h
      sqadd v30.8h, v30.8h, v9.8h
      # Convert to int8.
      sqxtn v12.8b, v12.8h
      sqxtn v16.8b, v16.8h
      sqxtn v20.8b, v20.8h
      sqxtn v24.8b, v24.8h
      sqxtn v28.8b, v28.8h
      sqxtn2 v12.16b, v14.8h
      sqxtn2 v16.16b, v18.8h
      sqxtn2 v20.16b, v22.8h
      sqxtn2 v24.16b, v26.8h
      sqxtn2 v28.16b, v30.8h
      # Min/max clamping.
      smin  v12.16b, v1.16b, v12.16b
      smin  v16.16b, v1.16b, v16.16b
      smin  v20.16b, v1.16b, v20.16b
      smin  v24.16b, v1.16b, v24.16b
      smin  v28.16b, v1.16b, v28.16b
      smax  v12.16b, v0.16b, v12.16b
      smax  v16.16b, v0.16b, v16.16b
      smax  v20.16b, v0.16b, v20.16b
      smax  v24.16b, v0.16b, v24.16b
      smax  v28.16b, v0.16b, v28.16b

      # Check whether full or partial store.
      cmp x1, 16
      b.lo .Ltail_8
      str q12, [x6], #16
      str q16, [x14], #16
      str q20, [x15], #16
      str q24, [x19], #16
      str q28, [x23], #16
      sub x3, x3, x2
      sub x9, x9, x2
      sub x10, x10, x2
      sub x11, x11, x2
      sub x12, x12, x2

      sub x1, x1, 16
      b.ne .Louter_loop
      b .Lreturn

.Ltail_8:
      tbz w1, 3, .Ltail_4
      str d12, [x6], #8
      str d16, [x14], #8
      str d20, [x15], #8
      str d24, [x19], #8
      str d28, [x23], #8
      ext v12.16b, v12.16b, v12.16b, 8
      ext v16.16b, v16.16b, v16.16b, 8
      ext v20.16b, v20.16b, v20.16b, 8
      ext v24.16b, v24.16b, v24.16b, 8
      ext v28.16b, v28.16b, v28.16b, 8


.Ltail_4:
      tbz w1, 2, .Ltail_2
      st1 {v12.s}[0], [x6], #4
      st1 {v16.s}[0], [x14], #4
      st1 {v20.s}[0], [x15], #4
      st1 {v24.s}[0], [x19], #4
      st1 {v28.s}[0], [x23], #4
      ext v12.16b, v12.16b, v12.16b, 4
      ext v16.16b, v16.16b, v16.16b, 4
      ext v20.16b, v20.16b, v20.16b, 4
      ext v24.16b, v24.16b, v24.16b, 4
      ext v28.16b, v28.16b, v28.16b, 4


.Ltail_2:
      tbz w1, 1, .Ltail_1
      st1 {v12.h}[0], [x6], #2
      st1 {v16.h}[0], [x14], #2
      st1 {v20.h}[0], [x15], #2
      st1 {v24.h}[0], [x19], #2
      st1 {v28.h}[0], [x23], #2
      ext v12.16b, v12.16b, v12.16b, 2
      ext v16.16b, v16.16b, v16.16b, 2
      ext v20.16b, v20.16b, v20.16b, 2
      ext v24.16b, v24.16b, v24.16b, 2
      ext v28.16b, v28.16b, v28.16b, 2


.Ltail_1:
      tbz w1, 0, .Lreturn
      st1 {v12.b}[0], [x6]
      st1 {v16.b}[0], [x14]
      st1 {v20.b}[0], [x15]
      st1 {v24.b}[0], [x19]
      st1 {v28.b}[0], [x23]

.Lreturn:
      # Restore the callee saved GP registers.
      ldp x27, x28, [sp, 224]
      ldp x25, x26, [sp, 192]
      ldp x23, x24, [sp, 160]
      ldp x21, x22, [sp, 128]
      ldp x19, x20, [sp, 96]

      # Restore callee saved q8-q15 registers.
      ldp d8, d9, [sp, 64]
      ldp d10, d11, [sp, 48]
      ldp d12, d13, [sp, 32]
      ldp d14, d15, [sp, 16]
      add sp, sp, 256
      ret
END_FUNCTION xnn_qs8_qc8w_gemm_minmax_fp32_ukernel_5x16c4__asm_aarch64_neondot_ld32_2