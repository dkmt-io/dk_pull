load("@rules_cc//cc:defs.bzl", "cc_test")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_test(
    name = "pull_stream_test",
    srcs = [
        "collect_test.cc",
        "drain_test.cc",
        "protocol_checker_test.cc",
        "values_test.cc",
    ],
    deps = [
        "//dk_pull/event",
        "//dk_pull/pull_stream",
        "//third_party/gtest",
        "//third_party/gtest:gtest_main",
    ],
)
