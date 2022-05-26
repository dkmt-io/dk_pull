workspace(name = "dk_pull")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "io_bazel_rules_go",
    remote = "https://github.com/bazelbuild/rules_go",
    tag = "v0.32.0",
)

git_repository(
    name = "com_google_protobuf",
    remote = "https://github.com/protocolbuffers/protobuf",
    tag = "v3.21.0",
)

git_repository(
    name = "com_github_bazelbuild_buildtools",
    remote = "https://github.com/bazelbuild/buildtools",
    tag = "5.1.0",
)

git_repository(
    name = "com_github_gflags_gflags",
    remote = "https://github.com/gflags/gflags",
    tag = "v2.2.2",
)

git_repository(
    name = "com_github_google_glog",
    remote = "https://github.com/google/glog",
    tag = "v0.6.0",
)

git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.11.0",
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")

go_register_toolchains(go_version = "1.18")

go_rules_dependencies()

protobuf_deps()
