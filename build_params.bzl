"""Build parameters for XNNPACK.

Arch-specific build parameters and utility functions.
"""

load(
    ":build_defs.bzl",
    "xnnpack_cc_library",
    "xnnpack_if_kleidiai_enabled",
    "xnnpack_select_if",
)

# buildifier: disable=out-of-order-load
def register_extension_info(**kwargs):
    pass

def xnnpack_src_to_target_name(src):
    src = src.split("/")[-1]
    src = src.split(".")[0]
    return src.replace("-", "_")

def xnnpack_list_sum(values, initial = 0):
    result = initial
    for value in values:
        result += value
    return result

def _create_params(
        cond = None,
        extra_deps = None,
        **kwargs):
    # Set defaults for unspecified parameters.
    extra_deps = extra_deps or []

    return struct(
        cond = cond,
        extra_deps = extra_deps,
        extra_kwargs = kwargs,
    )

def xnnpack_copts_for_arch(arch):
    params = XNNPACK_PARAMS_FOR_ARCH[arch]
    kwargs = params.extra_kwargs
    return (kwargs.get("copts", []) +
            xnnpack_select_if(
                "//build_config:x86",
                kwargs.get("gcc_x86_copts", []),
            ))

def xnnpack_target_compatible_with_for_arch(arch):
    params = XNNPACK_PARAMS_FOR_ARCH[arch]
    return xnnpack_select_if(
        cond = params.cond,
        val_false = ["@platforms//:incompatible"],
        val_true = [],
    )

def xnnpack_cond_for_arch(arch):
    return XNNPACK_PARAMS_FOR_ARCH[arch].cond

def xnnpack_cc_library_for_arch(arch, name, deps = None, **kwargs):
    """Creates an xnnpack_cc_library target for the given `arch` name.

    Args:
      arch: The name of the architecture to create the target for.
      name: The name of the target to create.
      deps: The list of dependencies to add to the target.
      **kwargs: Other arguments to pass to the xnnpack_cc_library target.
    """

    # Get the params for the given arch.
    params = XNNPACK_PARAMS_FOR_ARCH[arch]

    # Set defaults for unspecified parameters.
    deps = (deps or []) + params.extra_deps

    # Merge the kwargs with the extra args from the params.
    for key, val in params.extra_kwargs.items():
        if key in kwargs:
            kwargs[key] += val
        else:
            kwargs[key] = val

    xnnpack_cc_library(
        name = name,
        target_compatible_with = xnnpack_select_if(
            cond = params.cond,
            val_false = ["@platforms//:incompatible"],
            val_true = [],
        ),
        deps = deps,
        **kwargs
    )

register_extension_info(
    extension = xnnpack_cc_library_for_arch,
    label_regex_for_dep = "{extension_name}",
)

_XNNPACK_SIMD_ARCH_COPT_MAPPING = {
    "avx": xnnpack_select_if("//build_config:x86", ["-mavx"]),
    "avx2": xnnpack_select_if("//build_config:x86", ["-mavx2"]),
    "avx512f": xnnpack_select_if("//build_config:x86", ["-mavx512f"]),
    "avx512skx": xnnpack_select_if(
        "//build_config:x86",
        [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
        ],
    ),
    "avx512fp16": xnnpack_select_if(
        "//:avx512fp16_enabled",
        [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
            "-mgfni",
            "-mavx512fp16",
        ],
    ),
    "fma3": xnnpack_select_if("//build_config:x86", ["-mfma"]),
    "neon": select({
        "//build_config:aarch32": [
            "-marm",
            "-march=armv7-a",
            "-mfpu=neon",
        ],
        "//conditions:default": [],
    }),
    "neonfp16arith": select({
        "//build_config:aarch32": [
            "-marm",
            "-march=armv8.2-a+fp16",
            "-mfpu=neon-fp-armv8",
        ],
        "//build_config:aarch64": ["-march=armv8.2-a+fp16"],
        "//conditions:default": [],
    }),
    "sse2": xnnpack_select_if("//build_config:x86", ["-msse2"]),
    "sse41": xnnpack_select_if("//build_config:x86", ["-msse4.1"]),
    "hvx": xnnpack_select_if("//build_config:hexagon", ["-mhvx-ieee-fp"]),
}

def xnnpack_simd_copts_for_arch(arch):
    return _XNNPACK_SIMD_ARCH_COPT_MAPPING.get(arch, [])

def xnnpack_simd_f32_archs():
    return ["avx", "avx2", "avx512f", "fma3", "hvx", "neon", "scalar", "sse2", "sse2fma", "wasmsimd", "wasmrelaxedsimd"]

def xnnpack_simd_f16_archs():
    return ["scalar", "neonfp16arith", "avx512fp16"]

def xnnpack_simd_s16_archs():
    return ["avx2", "avx512skx", "neon", "scalar", "sse41", "wasmsimd"]

def xnnpack_simd_s32_archs():
    return ["avx2", "avx512f", "neon", "scalar", "sse41", "hvx", "wasmsimd"]

def xnnpack_simd_s8_archs():
    return ["scalar", "sse41", "neon", "hvx", "wasmsimd"]

def xnnpack_simd_u8_archs():
    return ["scalar", "sse2", "neon", "hvx", "wasmsimd"]

def xnnpack_archs():
    return XNNPACK_PARAMS_FOR_ARCH.keys()

def _x86_align_stack(alignment):
    """Adds `copt`s to align the stack to the given `alignment` for `x86`."""
    log2_alignment = 0
    temp = alignment
    for _ in range(64):
        if temp == 0:
            break
        log2_alignment += 1
        temp = temp // 2
    return select({
        "//build_config:clang_and_not_fuchsia_or_ios": [
            "-mstack-alignment=" + str(alignment),
            "-fomit-frame-pointer",
            "-mstackrealign",
        ],
        "//build_config:gcc": [
            "-mpreferred-stack-boundary=" + str(log2_alignment),
            "-fomit-frame-pointer",
            "-mstackrealign",
            "-mincoming-stack-boundary=4",
        ],
        "//conditions:default": [],
    })

XNNPACK_PARAMS_FOR_ARCH = {
    # Scalar.
    "scalar": _create_params(
        copts = xnnpack_select_if(
            "//build_config:aarch32",
            ["-marm"],
        ) + [
            "-fno-fast-math",
            "-fno-math-errno",
            "-ffp-contract=off",
        ],
        extra_deps = [
            "//src/configs:config_hdrs",
            "@FXdiv",
        ],
    ),

    # Emscripten.
    "wasm": _create_params(
        cond = "//build_config:emscripten_wasm",
        copts = [
            "-fno-fast-math",
            "-fno-math-errno",
        ],
    ),
    "wasmsimd": _create_params(
        cond = "//build_config:emscripten_wasmsimd",
        copts = [
            "-fno-fast-math",
            "-fno-math-errno",
        ],
    ),
    "wasmrelaxedsimd": _create_params(
        cond = "//build_config:emscripten_wasmrelaxedsimd",
        copts = [
            "-fno-fast-math",
            "-fno-math-errno",
            "-mfp16",
        ],
    ),
    "wasm32": _create_params(
        cond = "//build_config:emscripten_wasm",
    ),
    "wasmsimd32": _create_params(
        cond = "//build_config:emscripten_wasmsimd",
    ),
    "wasmrelaxedsimd32": _create_params(
        cond = "//build_config:emscripten_wasmrelaxedsimd",
    ),

    # ARM.
    "armsimd32": _create_params(
        cond = "//build_config:aarch32",
        copts = [
            "-marm",
            "-march=armv6",
            "-mfpu=vfp",
            "-munaligned-access",
            "-fno-fast-math",
            "-fno-math-errno",
        ],
    ),
    "fp16arith": _create_params(
        cond = "//:arm_fp16_scalar_enabled",
        copts = select({
            "//build_config:aarch32": [
                "-marm",
                "-march=armv8.2-a+fp16",
                # GCC emits wrong directives for assembler with -mfpu=fp-armv8
                "-mfpu=neon-fp-armv8",
                # For vsqrth_f16 polyfill using sqrtf
                "-fno-math-errno",
                # For vminh_f16/vmaxh_f16 polyfills using compare + select
                "-ffinite-math-only",
            ],
            "//build_config:aarch64": ["-march=armv8.2-a+fp16"],
            "//conditions:default": [],
        }),
        extra_deps = xnnpack_if_kleidiai_enabled([
            "@KleidiAI//kai/ukernels/matmul",
        ]),
    ),
    "neon": _create_params(
        cond = "//build_config:arm",
        copts = xnnpack_select_if(
            "//build_config:aarch32",
            [
                "-marm",
                "-march=armv7-a",
                "-mfpu=neon",
            ],
        ),
        extra_deps = xnnpack_if_kleidiai_enabled([
            "@KleidiAI//kai/ukernels/matmul",
        ]),
    ),
    "neon_aarch64": _create_params(
        cond = "//build_config:aarch64",
        extra_deps = xnnpack_if_kleidiai_enabled([
            "@KleidiAI//kai/ukernels/matmul",
        ]),
    ),
    "neonfp16": _create_params(
        cond = "//build_config:arm",
        copts = xnnpack_select_if(
            "//build_config:aarch32",
            [
                "-marm",
                "-march=armv7-a",
                "-mfpu=neon-fp16",
            ],
        ),
    ),
    "neonfma": _create_params(
        cond = "//build_config:arm",
        copts = xnnpack_select_if(
            "//build_config:aarch32",
            [
                "-marm",
                "-march=armv7-a",
                "-mfpu=neon-vfpv4",
            ],
        ),
    ),
    "neonfma_aarch64": _create_params(
        cond = "//build_config:aarch64",
    ),
    "neonv8": _create_params(
        cond = "//build_config:arm",
        copts = xnnpack_select_if(
            "//build_config:aarch32",
            [
                "-marm",
                "-march=armv8-a",
                "-mfpu=neon-fp-armv8",
            ],
        ),
    ),
    "neonfp16arith": _create_params(
        cond = "//:arm_fp16_vector_enabled",
        copts = select({
            "//build_config:aarch32": [
                "-marm",
                "-march=armv8.2-a+fp16",
                "-mfpu=neon-fp-armv8",
            ],
            "//build_config:aarch64": ["-march=armv8.2-a+fp16"],
            "//conditions:default": [],
        }),
    ),
    "neonfp16arith_aarch64": _create_params(
        cond = "//:arm_aarch64_fp16_vector_enabled",
        copts = ["-march=armv8.2-a+fp16"],
    ),
    "neonbf16": _create_params(
        cond = "//build_config:aarch64",
        copts = ["-march=armv8.2-a+bf16"],
    ),
    "neondotfp16arith": _create_params(
        cond = "//:arm_neondotfp16_enabled",
        copts = select({
            "//build_config:aarch32": [
                "-marm",
                "-march=armv8.2-a+dotprod+fp16",
                "-mfpu=neon-fp-armv8",
            ],
            "//build_config:aarch64": ["-march=armv8.2-a+dotprod+fp16"],
            "//conditions:default": [],
        }),
    ),
    "neondot": _create_params(
        cond = "//:arm_dotprod_enabled",
        copts = select({
            "//build_config:aarch32": [
                "-marm",
                "-march=armv8.2-a+dotprod",
                "-mfpu=neon-fp-armv8",
            ],
            "//build_config:aarch64": ["-march=armv8.2-a+dotprod"],
            "//conditions:default": [],
        }),
    ),
    "neondot_aarch64": _create_params(
        cond = "//:arm_aarch64_dotprod_enabled",
        copts = ["-march=armv8.2-a+dotprod"],
        extra_deps = xnnpack_if_kleidiai_enabled([
            "@KleidiAI//kai/ukernels/matmul:matmul",
        ]),
    ),
    "neoni8mm": _create_params(
        cond = "//:arm_i8mm_enabled",
        copts = ["-march=armv8.2-a+i8mm+fp16"],
        extra_deps = xnnpack_if_kleidiai_enabled([
            "@KleidiAI//kai/ukernels/matmul:matmul",
        ]),
    ),
    "neonsme": _create_params(
        cond = "//:arm_sme_enabled",
        copts = ["-march=armv8.2-a+sve+sve2"],
    ),
    "neonsme2": _create_params(
        cond = "//:arm_sme2_enabled",
        copts = ["-march=armv8.2-a+sve+sve2"],
        extra_deps = xnnpack_if_kleidiai_enabled([
            "@KleidiAI//kai/ukernels/matmul:matmul",
        ]),
    ),
    "aarch32": _create_params(
        cond = "//build_config:aarch32",
        copts = [
            "-marm",
            "-march=armv8.2-a+dotprod+fp16",
            "-mfpu=neon-fp-armv8",
        ],
    ),
    "aarch64": _create_params(
        cond = "//build_config:aarch64",
        copts = ["-march=armv8.2-a+fp16+dotprod"],
    ),

    # X86.
    "sse": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(16),
        gcc_x86_copts = [
            "-msse",
            "-mno-sse2",
        ],
        msvc_x86_32_copts = ["/arch:SSE"],
        msvc_x86_64_copts = ["/arch:SSE"],
    ),
    "sse2": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(16),
        gcc_x86_copts = [
            "-msse2",
            "-mno-sse3",
        ],
        msvc_x86_32_copts = ["/arch:SSE2"],
        msvc_x86_64_copts = ["/arch:SSE2"],
    ),
    "sse2fma": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(16),
        gcc_x86_copts = [
            "-msse2",
            "-mno-sse3",
        ],
        msvc_x86_32_copts = ["/arch:SSE2"],
        msvc_x86_64_copts = ["/arch:SSE2"],
    ),
    "ssse3": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(16),
        gcc_x86_copts = [
            "-mssse3",
            "-mno-sse4.1",
        ],
        msvc_x86_32_copts = ["/arch:SSE2"],
        msvc_x86_64_copts = ["/arch:SSE2"],
    ),
    "sse41": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(16),
        gcc_x86_copts = [
            "-msse4.1",
            "-mno-sse4.2",
        ],
        extra_deps = [
        ],
        msvc_x86_32_copts = ["/arch:SSE2"],
        msvc_x86_64_copts = ["/arch:SSE2"],
    ),
    "avx": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mavx",
            "-mno-avx2",
            "-mno-f16c",
            "-mno-fma",
        ],
        msvc_x86_32_copts = ["/arch:AVX"],
        msvc_x86_64_copts = ["/arch:AVX"],
    ),
    "avxvnni": _create_params(
        cond = "//:avxvnni_enabled",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mavx2",
            "-mavxvnni",
            "-mf16c",
            "-mfma",
        ],
        msvc_x86_32_copts = ["/arch:AVX"],
        msvc_x86_64_copts = ["/arch:AVX"],
    ),
    "avxvnniint8": _create_params(
        cond = "//:avxvnniint8_enabled",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mavx2",
            "-mavxvnniint8",
            "-mf16c",
            "-mfma",
        ],
        msvc_x86_32_copts = ["/arch:AVX"],
        msvc_x86_64_copts = ["/arch:AVX"],
    ),
    "f16c": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mf16c",
            "-mno-avx2",
            "-mno-fma",
        ],
        msvc_x86_32_copts = ["/arch:AVX"],
        msvc_x86_64_copts = ["/arch:AVX"],
    ),
    "fma3": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mno-avx2",
        ],
        msvc_x86_32_copts = ["/arch:AVX"],
        msvc_x86_64_copts = ["/arch:AVX"],
    ),
    "avx2": _create_params(
        cond = "//build_config:x86",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx2",
        ],
        msvc_x86_32_copts = ["/arch:AVX2"],
        msvc_x86_64_copts = ["/arch:AVX2"],
    ),
    "avx512f": _create_params(
        cond = "//:avx512f_enabled",
        copts = _x86_align_stack(64),
        gcc_x86_copts = [
            "-mavx512f",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx512skx": _create_params(
        cond = "//:avx512skx_enabled",
        copts = _x86_align_stack(64),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx256skx": _create_params(
        cond = "//:avx256skx_enabled",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx512vbmi": _create_params(
        cond = "//:avx512vbmi_enabled",
        copts = _x86_align_stack(64),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vbmi",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx512vnni": _create_params(
        cond = "//:avx512vnni_enabled",
        copts = _x86_align_stack(64),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx256vnni": _create_params(
        cond = "//:avx256vnni_enabled",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx512vnnigfni": _create_params(
        cond = "//:avx512vnnigfni_enabled",
        copts = _x86_align_stack(64),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
            "-mgfni",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx256vnnigfni": _create_params(
        cond = "//:avx256vnnigfni_enabled",
        copts = _x86_align_stack(32),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
            "-mgfni",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx512amx": _create_params(
        cond = "//:avx512amx_enabled",
        copts = _x86_align_stack(64),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
            "-mgfni",
            "-mamx-tile",
            "-mamx-int8",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx512fp16": _create_params(
        cond = "//:avx512fp16_enabled",
        copts = _x86_align_stack(64),
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
            "-mgfni",
            "-mavx512fp16",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
    "avx512bf16": _create_params(
        cond = "//:avx512bf16_enabled",
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
            "-mgfni",
            "-mavx512bf16",
        ],
        msvc_x86_32_copts = ["/arch:AVX512"],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),

    # RISC-V.
    "rvv": _create_params(
        cond = "//:riscv_vector_enabled",
        copts = [
            "-march=rv64gcv",
            "-mabi=lp64d",
        ],
    ),
    "rvvfp16arith": _create_params(
        cond = "//:riscv_fp16_vector_enabled",
        copts = [
            "-march=rv64gc_zvfh",
            "-mabi=lp64d",
        ],
    ),

    # Hexagon.
    "hexagon": _create_params(
        cond = "//:hvx_enabled",
        copts = [
            "-mhvx-ieee-fp",
        ],
        extra_deps = [],  # Extra deps for hexagon.
    ),
    "hvx": _create_params(
        cond = "//:hvx_enabled",
        copts = [
            "-mhvx-ieee-fp",
        ],
        extra_deps = [],  # Extra deps for hexagon.
    ),
    "amd64": _create_params(
        cond = "//build_config:x86_64",
        gcc_x86_copts = [
            "-mf16c",
            "-mfma",
            "-mavx512f",
            "-mavx512cd",
            "-mavx512bw",
            "-mavx512dq",
            "-mavx512vl",
            "-mavx512vnni",
            "-mgfni",
        ],
        msvc_x86_64_copts = ["/arch:AVX512"],
        mingw_copts = ["-fno-asynchronous-unwind-tables"],
        msys_copts = ["-fno-asynchronous-unwind-tables"],
    ),
}
