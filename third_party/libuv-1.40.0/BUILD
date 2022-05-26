load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = ["treat_warnings_as_errors"],
    licenses = ["notice"],
)

uv_sources = [
    "src/fs-poll.c",
    "src/idna.c",
    "src/inet.c",
    "src/random.c",
    "src/strscpy.c",
    "src/threadpool.c",
    "src/timer.c",
    "src/uv-common.c",
    "src/uv-data-getter-setters.c",
    "src/version.c",
]

uv_sources_unix = uv_sources + [
    "src/unix/async.c",
    "src/unix/core.c",
    "src/unix/dl.c",
    "src/unix/fs.c",
    "src/unix/getaddrinfo.c",
    "src/unix/getnameinfo.c",
    "src/unix/loop-watcher.c",
    "src/unix/loop.c",
    "src/unix/pipe.c",
    "src/unix/poll.c",
    "src/unix/process.c",
    "src/unix/random-devurandom.c",
    "src/unix/signal.c",
    "src/unix/stream.c",
    "src/unix/tcp.c",
    "src/unix/thread.c",
    "src/unix/tty.c",
    "src/unix/udp.c",
]

uv_sources_linux = uv_sources_unix + [
    "src/unix/proctitle.c",
    "src/unix/linux-core.c",
    "src/unix/linux-inotify.c",
    "src/unix/linux-syscalls.c",
    "src/unix/procfs-exepath.c",
    "src/unix/random-getrandom.c",
    "src/unix/random-sysctl-linux.c",
]

uv_sources_macos = uv_sources_unix + [
    "src/unix/proctitle.c",
    "src/unix/darwin-proctitle.c",
    "src/unix/darwin.c",
    "src/unix/fsevents.c",
    "src/unix/bsd-ifaddrs.c",
    "src/unix/kqueue.c",
    "src/unix/random-getentropy.c",
]

uv_sources_qnx = uv_sources_unix + [
    "src/unix/posix-hrtime.c",
    "src/unix/posix-poll.c",
    "src/unix/qnx.c",
    "src/unix/bsd-ifaddrs.c",
    "src/unix/no-proctitle.c",
    "src/unix/no-fsevents.c",
]

uv_defines = []

uv_defines_unix = uv_defines + [
    "_FILE_OFFSET_BITS=64",
    "_LARGEFILE_SOURCE",
]

uv_defines_linux = uv_defines_unix + [
    "_GNU_SOURCE",
    "_POSIX_C_SOURCE=200112",
]

uv_defines_macos = uv_defines_unix + [
    "_DARWIN_UNLIMITED_SELECT=1",
    "_DARWIN_USE_64_BIT_INODE=1",
]

uv_defines_qnx = uv_defines_unix + [
]

uv_libraries = [
]

uv_libraries_unix = uv_libraries + [
]

uv_libraries_linux = uv_libraries_unix + [
    "-ldl",
    "-lrt",
    "-lutil",
    "-lpthread",
]

uv_libraries_macos = uv_libraries_unix + [
    "-lpthread",
]

uv_libraries_qnx = uv_libraries_unix + [
    "-lsocket",
]

uv_test_sources = [
    "test/runner-unix.c",
    "test/blackhole-server.c",
    "test/echo-server.c",
    "test/run-tests.c",
    "test/runner.c",
    "test/test-active.c",
    "test/test-async-null-cb.c",
    "test/test-async.c",
    "test/test-barrier.c",
    "test/test-callback-order.c",
    "test/test-callback-stack.c",
    "test/test-close-fd.c",
    "test/test-close-order.c",
    "test/test-condvar.c",
    "test/test-connect-unspecified.c",
    "test/test-connection-fail.c",
    "test/test-cwd-and-chdir.c",
    "test/test-default-loop-close.c",
    "test/test-delayed-accept.c",
    "test/test-dlerror.c",
    "test/test-eintr-handling.c",
    "test/test-embed.c",
    "test/test-emfile.c",
    "test/test-env-vars.c",
    "test/test-error.c",
    "test/test-fail-always.c",
    "test/test-fork.c",
    "test/test-fs-copyfile.c",
    "test/test-fs-event.c",
    "test/test-fs-poll.c",
    "test/test-fs.c",
    "test/test-fs-readdir.c",
    "test/test-fs-fd-hash.c",
    "test/test-fs-open-flags.c",
    "test/test-get-currentexe.c",
    "test/test-get-loadavg.c",
    "test/test-get-memory.c",
    "test/test-get-passwd.c",
    "test/test-getaddrinfo.c",
    "test/test-gethostname.c",
    "test/test-getnameinfo.c",
    "test/test-getsockname.c",
    "test/test-getters-setters.c",
    "test/test-gettimeofday.c",
    "test/test-handle-fileno.c",
    "test/test-homedir.c",
    "test/test-hrtime.c",
    "test/test-idle.c",
    "test/test-idna.c",
    "test/test-ip4-addr.c",
    "test/test-ip6-addr.c",
    "test/test-ipc-heavy-traffic-deadlock-bug.c",
    "test/test-ipc-send-recv.c",
    "test/test-ipc.c",
    "test/test-loop-alive.c",
    "test/test-loop-close.c",
    "test/test-loop-configure.c",
    "test/test-loop-handles.c",
    "test/test-loop-stop.c",
    "test/test-loop-time.c",
    "test/test-metrics.c",
    "test/test-multiple-listen.c",
    "test/test-mutexes.c",
    "test/test-osx-select.c",
    "test/test-pass-always.c",
    "test/test-ping-pong.c",
    "test/test-pipe-bind-error.c",
    "test/test-pipe-close-stdout-read-stdin.c",
    "test/test-pipe-connect-error.c",
    "test/test-pipe-connect-multiple.c",
    "test/test-pipe-connect-prepare.c",
    "test/test-pipe-getsockname.c",
    "test/test-pipe-pending-instances.c",
    "test/test-pipe-sendmsg.c",
    "test/test-pipe-server-close.c",
    "test/test-pipe-set-fchmod.c",
    "test/test-pipe-set-non-blocking.c",
    "test/test-platform-output.c",
    "test/test-poll-close-doesnt-corrupt-stack.c",
    "test/test-poll-close.c",
    "test/test-poll-closesocket.c",
    "test/test-poll-oob.c",
    "test/test-poll.c",
    "test/test-process-priority.c",
    "test/test-process-title-threadsafe.c",
    "test/test-process-title.c",
    "test/test-queue-foreach-delete.c",
    "test/test-random.c",
    "test/test-ref.c",
    "test/test-run-nowait.c",
    "test/test-run-once.c",
    "test/test-semaphore.c",
    "test/test-shutdown-close.c",
    "test/test-shutdown-eof.c",
    "test/test-shutdown-twice.c",
    "test/test-signal-multiple-loops.c",
    "test/test-signal-pending-on-close.c",
    "test/test-signal.c",
    "test/test-socket-buffer-size.c",
    "test/test-spawn.c",
    "test/test-stdio-over-pipes.c",
    "test/test-strscpy.c",
    "test/test-tcp-alloc-cb-fail.c",
    "test/test-tcp-bind-error.c",
    "test/test-tcp-bind6-error.c",
    "test/test-tcp-close-accept.c",
    "test/test-tcp-close-while-connecting.c",
    "test/test-tcp-close.c",
    "test/test-tcp-close-reset.c",
    "test/test-tcp-connect-error-after-write.c",
    "test/test-tcp-connect-error.c",
    "test/test-tcp-connect-timeout.c",
    "test/test-tcp-connect6-error.c",
    "test/test-tcp-create-socket-early.c",
    "test/test-tcp-flags.c",
    "test/test-tcp-oob.c",
    "test/test-tcp-open.c",
    "test/test-tcp-read-stop.c",
    "test/test-tcp-read-stop-start.c",
    "test/test-tcp-shutdown-after-write.c",
    "test/test-tcp-try-write.c",
    "test/test-tcp-try-write-error.c",
    "test/test-tcp-unexpected-read.c",
    "test/test-tcp-write-after-connect.c",
    "test/test-tcp-write-fail.c",
    "test/test-tcp-write-queue-order.c",
    "test/test-tcp-write-to-half-open-connection.c",
    "test/test-tcp-writealot.c",
    "test/test-test-macros.c",
    "test/test-thread-equal.c",
    "test/test-thread.c",
    "test/test-threadpool-cancel.c",
    "test/test-threadpool.c",
    "test/test-timer-again.c",
    "test/test-timer-from-check.c",
    "test/test-timer.c",
    "test/test-tmpdir.c",
    "test/test-tty-duplicate-key.c",
    "test/test-tty-escape-sequence-processing.c",
    "test/test-tty.c",
    "test/test-udp-alloc-cb-fail.c",
    "test/test-udp-bind.c",
    "test/test-udp-connect.c",
    "test/test-udp-create-socket-early.c",
    "test/test-udp-dgram-too-big.c",
    "test/test-udp-ipv6.c",
    "test/test-udp-mmsg.c",
    "test/test-udp-multicast-interface.c",
    "test/test-udp-multicast-interface6.c",
    "test/test-udp-multicast-join.c",
    "test/test-udp-multicast-join6.c",
    "test/test-udp-multicast-ttl.c",
    "test/test-udp-open.c",
    "test/test-udp-options.c",
    "test/test-udp-send-and-recv.c",
    "test/test-udp-send-hang-loop.c",
    "test/test-udp-send-immediate.c",
    "test/test-udp-sendmmsg-error.c",
    "test/test-udp-send-unreachable.c",
    "test/test-udp-try-send.c",
    "test/test-uname.c",
    "test/test-walk-handles.c",
    "test/test-watcher-cross-stop.c",
]

cc_library(
    name = "libuv",
    srcs = select({
        "@platforms//os:linux": uv_sources_linux,
        "@platforms//os:macos": uv_sources_macos,
        "@platforms//os:qnx": uv_sources_qnx,
    }),
    hdrs = glob([
        "include/**/*.h",
        "src/**/*.h",
    ]),
    includes = ["include"],
    linkopts = select({
        "@platforms//os:linux": uv_libraries_linux,
        "@platforms//os:macos": uv_libraries_macos,
        "@platforms//os:qnx": uv_libraries_qnx,
    }),
    local_defines = select({
        "@platforms//os:linux": uv_defines_linux,
        "@platforms//os:macos": uv_defines_macos,
        "@platforms//os:qnx": uv_defines_qnx,
    }),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "uv_test_lib",
    hdrs = glob([
        "src/**/*.c",
        "test/**/*.h",
    ]),
    deps = [":libuv"],
)

cc_binary(
    name = "uv_test",
    srcs = uv_test_sources,
    local_defines = select({
        "@platforms//os:linux": uv_defines_linux,
        "@platforms//os:macos": uv_defines_macos,
        "@platforms//os:qnx": uv_defines_qnx,
    }),
    deps = [":uv_test_lib"],
)
