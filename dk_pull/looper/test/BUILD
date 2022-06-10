load("@rules_cc//cc:defs.bzl", "cc_test")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_test(
    name = "looper_test",
    srcs = ["looper_test.cc"],
    deps = [
        "//dk_pull/event",
        "//dk_pull/looper",
        "//third_party/gtest",
        "//third_party/gtest:gtest_main",
    ],
)
