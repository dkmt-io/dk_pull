cmake_minimum_required(VERSION 3.10)

add_library(dk_pull_event_loop
  event_loop.cc
  event_loop.h
)
set_target_properties(dk_pull_event_loop PROPERTIES CXX_CLANG_TIDY "clang-tidy")
target_link_libraries(dk_pull_event_loop
  glog
  gflags_nothreads
  uv_a
  dl
  pthread
)

add_executable(event_loop_test
  test/event_loop_test.cc
)
set_target_properties(event_loop_test PROPERTIES CXX_CLANG_TIDY "clang-tidy")
target_link_libraries(event_loop_test
  dk_pull_event_loop
  gtest
  gtest_main
)
add_test(
  NAME event_loop_test
  COMMAND event_loop_test
)
