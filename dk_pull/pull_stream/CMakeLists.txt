cmake_minimum_required(VERSION 3.10)

add_library(dk_pull_pull_stream
  sinks/abort_now_sink.cc
  sinks/abort_now_sink.h
  sinks/drain.cc
  sinks/drain.h
  sources/count.cc
  sources/count.h
  sources/values.cc
  sources/values.h
  protocol_checker.cc
  protocol_checker.h
  pull.cc
  pull.h
)
target_link_libraries(dk_pull_pull_stream
  glog
  gflags_nothreads
  uv_a
  dl
  pthread
  dk_pull_common
  dk_pull_looper
  dk_pull_types
)

add_executable(pull_stream_test
  test/drain_test.cc
  test/protocol_checker_test.cc
  test/values_test.cc
)
target_link_libraries(pull_stream_test
  dk_pull_pull_stream
  gtest
  gtest_main
)
add_test(
  NAME pull_stream_test
  COMMAND pull_stream_test
)
