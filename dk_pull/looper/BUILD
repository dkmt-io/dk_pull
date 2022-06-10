load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "looper",
    srcs = ["looper.cc"],
    hdrs = ["looper.h"],
    deps = [
        "//dk_pull/common",
        "//third_party/glog",
    ],
)
