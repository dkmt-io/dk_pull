load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "common",
    srcs = [
        "finally.cc",
        "split_string.cc",
        "uncopyable.cc",
    ],
    hdrs = [
        "finally.h",
        "split_string.h",
        "uncopyable.h",
    ],
    deps = [
        "//third_party/glog",
    ],
)
