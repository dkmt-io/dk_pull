load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "event_loop",
    srcs = ["event_loop.cc"],
    hdrs = ["event_loop.h"],
    deps = [
        "//dk_pull/common",
        "//third_party/glog",
        "//third_party/libuv",
    ],
)
