load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//dk_pull/event:__subpackages__"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "timer",
    srcs = ["timer.cc"],
    hdrs = ["timer.h"],
    deps = ["//dk_pull/common"],
)

cc_library(
    name = "uv_timer",
    srcs = ["uv_timer.cc"],
    hdrs = ["uv_timer.h"],
    deps = [
        ":timer",
        "//third_party/libuv",
    ],
)
