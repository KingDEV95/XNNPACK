# Copyright (c) Facebook, Inc. and its affiliates.
# All rights reserved.
#
# Copyright 2019 Google LLC
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.

CMAKE_MINIMUM_REQUIRED(VERSION 3.5 FATAL_ERROR)

PROJECT(pthreadpool-download NONE)

# Set file timestamps to the time of extraction.
IF(POLICY CMP0135)
  CMAKE_POLICY(SET CMP0135 NEW)
ENDIF()

# LINT.IfChange
INCLUDE(ExternalProject)
ExternalProject_Add(pthreadpool
  URL https://github.com/google/pthreadpool/archive/4e80ca24521aa0fb3a746f9ea9c3eaa20e9afbb0.zip
  URL_HASH SHA256=6d373fa7e2b899605fc3b6e72171a71bccbaf9d4d596b7f514535c4ffb966b3b
  SOURCE_DIR "${CMAKE_BINARY_DIR}/pthreadpool-source"
  BINARY_DIR "${CMAKE_BINARY_DIR}/pthreadpool"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)
# LINT.ThenChange(../MODULE.bazel:pthreadpool)
