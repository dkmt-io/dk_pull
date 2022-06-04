cmake_minimum_required(VERSION 3.10)

add_library(dk_pull_event
  event_loop.cc
  event_loop.h
  set_immediate.cc
  set_immediate.h
  set_interval.cc
  set_interval.h
  set_timeout.cc
  set_timeout.h
  timer.cc
  timer.h
)
target_link_libraries(dk_pull_event
  glog
  gflags_nothreads
  uv_a
  dl
  pthread
)

add_executable(event_loop_test
  test/event_loop_test.cc
)
target_link_libraries(event_loop_test
  dk_pull_event
  gtest
  gtest_main
)
add_test(
  NAME event_loop_test
  COMMAND event_loop_test
)

add_executable(set_immediate_test
  test/set_immediate_test.cc
)
target_link_libraries(set_immediate_test
  dk_pull_event
  gtest
  gtest_main
)
add_test(
  NAME set_immediate_test
  COMMAND set_immediate_test
)

add_executable(timer_test
  test/timer_test.cc
)
target_link_libraries(timer_test
  dk_pull_event
  gtest
  gtest_main
)
add_test(
  NAME timer_test
  COMMAND timer_test
)
