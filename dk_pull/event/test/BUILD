load("@rules_cc//cc:defs.bzl", "cc_test")

package(
    default_visibility = ["//dk_pull/event:__subpackages__"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_test(
    name = "event_loop_test",
    srcs = ["event_loop_test.cc"],
    deps = [
        "//dk_pull/event/event_loop",
        "//third_party/gtest",
        "//third_party/gtest:gtest_main",
    ],
)

cc_test(
    name = "timer_test",
    srcs = ["timer_test.cc"],
    deps = [
        "//dk_pull/event/event_loop",
        "//third_party/gtest",
        "//third_party/gtest:gtest_main",
    ],
)
