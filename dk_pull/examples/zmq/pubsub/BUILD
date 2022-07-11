load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    linkstatic = True,
    deps = [
        "//dk_pull/common",
        "//third_party/gflags",
        "//third_party/glog",
        "//third_party/zeromq:libzmq",
    ],
)

cc_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        "//dk_pull/common",
        "//third_party/gflags",
        "//third_party/glog",
        "//third_party/zeromq:libzmq",
    ],
)
