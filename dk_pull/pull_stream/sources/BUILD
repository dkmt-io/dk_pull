load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "sources",
    srcs = [
        "count.cc",
        "values.cc",
    ],
    hdrs = [
        "count.h",
        "values.h",
    ],
    deps = [
        "//dk_pull/common",
        "//dk_pull/types",
        "//third_party/glog",
    ],
)
