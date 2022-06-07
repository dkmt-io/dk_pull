cmake_minimum_required(VERSION 3.10)

add_library(dk_pull_pull_stream
  sinks/drain.cc
  sinks/drain.h
  sources/values.cc
  sources/values.h
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
