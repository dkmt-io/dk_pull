load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "gtest",
    visibility = ["//visibility:public"],
    deps = ["@com_google_googletest//:gtest"],
)

cc_library(
    name = "gtest_main",
    visibility = ["//visibility:public"],
    deps = ["@com_google_googletest//:gtest_main"],
)
