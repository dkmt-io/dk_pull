load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//dk_pull/event:__subpackages__"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "event_loop",
    srcs = [
        "event_loop.cc",
        "uv_event_loop.cc",
    ],
    hdrs = [
        "event_loop.h",
        "uv_event_loop.h",
    ],
    deps = [
        "//dk_pull/event:event_provider",
        "//dk_pull/event/timer",
        "//dk_pull/event/timer:uv_timer",
    ],
)
