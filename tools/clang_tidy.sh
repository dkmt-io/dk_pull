#!/bin/sh

bazel build //...                                                          \
  --aspects @bazel_clang_tidy//clang_tidy:clang_tidy.bzl%clang_tidy_aspect \
  --output_groups=report                                                   \
  --@bazel_clang_tidy//:clang_tidy_config=//:clang_tidy_config
