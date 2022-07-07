load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "libzmq",
    visibility = ["//visibility:public"],
    deps = ["@zeromq-4.3.4//:libzmq"],
)
