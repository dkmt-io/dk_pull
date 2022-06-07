cmake_minimum_required(VERSION 3.10)

add_library(dk_pull_looper
  looper.cc
  looper.h
)
target_link_libraries(dk_pull_looper
  glog
  gflags_nothreads
  uv_a
  dl
  pthread
  dk_pull_common
)

add_executable(looper_test
  test/looper_test.cc
)
target_link_libraries(looper_test
  dk_pull_event
  dk_pull_looper
  gtest
  gtest_main
)
add_test(
  NAME looper_test
  COMMAND looper_test
)
