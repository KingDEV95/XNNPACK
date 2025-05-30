// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <stddef.h>
#include <stdint.h>

#include "include/xnnpack.h"
#include "src/xnnpack/common.h"
#include "src/xnnpack/log.h"


// This function is defined inline when logging is disabled
const char* xnn_datatype_to_string(enum xnn_datatype type) {
  switch (type) {
    case xnn_datatype_invalid:
      return "Invalid";
    case xnn_datatype_fp32:
      return "FP32";
    case xnn_datatype_fp16:
      return "FP16";
    case xnn_datatype_bf16:
      return "BF16";
    case xnn_datatype_pfp16:
      return "PFP16";
    case xnn_datatype_pfp32:
      return "PFP32";
    case xnn_datatype_qint8:
      return "QINT8";
    case xnn_datatype_pqint8:
      return "PQINT8";
    case xnn_datatype_quint8:
      return "QUINT8";
    case xnn_datatype_qint32:
      return "QINT32";
    case xnn_datatype_qcint4:
      return "QCINT4";
    case xnn_datatype_qcint8:
      return "QCINT8";
    case xnn_datatype_qcint32:
      return "QCINT32";
    case xnn_datatype_qdint8:
      return "QDINT8";
    case xnn_datatype_qduint8:
      return "QDUINT8";
    case xnn_datatype_qpint8:
      return "QPINT8";
    case xnn_datatype_int32:
     return "INT32";
    case xnn_datatype_qbint4:
     return "QBINT4";
  }
  XNN_UNREACHABLE;
  return NULL;
}
