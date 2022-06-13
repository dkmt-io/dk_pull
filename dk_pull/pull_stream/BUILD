load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "pull_stream",
    deps = [
        "//dk_pull/pull_stream:protocol_checker",
        "//dk_pull/pull_stream:pull",
        "//dk_pull/pull_stream/sinks",
        "//dk_pull/pull_stream/sources",
        "//dk_pull/pull_stream/throughs",
    ],
)

cc_library(
    name = "pull",
    srcs = ["pull.cc"],
    hdrs = ["pull.h"],
    deps = [
        "//dk_pull/common",
        "//dk_pull/types",
        "//third_party/glog",
    ],
)

cc_library(
    name = "protocol_checker",
    srcs = ["protocol_checker.cc"],
    hdrs = ["protocol_checker.h"],
    deps = [
        "//dk_pull/common",
        "//dk_pull/types",
        "//third_party/glog",
    ],
)
