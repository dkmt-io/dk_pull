load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "types",
    srcs = [
        "duplex.cc",
        "end_or_error.cc",
        "sink.cc",
        "source.cc",
        "through.cc",
    ],
    hdrs = [
        "duplex.h",
        "end_or_error.h",
        "sink.h",
        "source.h",
        "through.h",
    ],
    deps = [
        "//dk_pull/common",
        "//third_party/glog",
    ],
)
