load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "libuv",
    visibility = ["//visibility:public"],
    deps = ["//third_party/libuv-1.40.0:libuv"],
)
