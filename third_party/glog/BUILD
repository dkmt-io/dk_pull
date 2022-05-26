load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

cc_library(
    name = "glog",
    visibility = ["//visibility:public"],
    deps = ["@com_github_google_glog//:glog"],
)
