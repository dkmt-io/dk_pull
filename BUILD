load("@com_github_bazelbuild_buildtools//buildifier:def.bzl", "buildifier")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

buildifier(
    name = "buildifier",
)

filegroup(
    name = "clang_tidy_config",
    data = [".clang-tidy"],
    visibility = ["//visibility:public"],
)
