load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "event",
    srcs = [
        "event_loop.cc",
        "set_immediate.cc",
        "set_interval.cc",
        "set_timeout.cc",
        "timer.cc",
    ],
    hdrs = [
        "event_loop.h",
        "set_immediate.h",
        "set_interval.h",
        "set_timeout.h",
        "timer.h",
    ],
    deps = [
        "//dk_pull/common",
        "//third_party/glog",
        "//third_party/libuv",
    ],
)
