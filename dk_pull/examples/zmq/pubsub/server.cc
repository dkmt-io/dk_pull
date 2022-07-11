/*******************************************************************************
 * Copyright (C) 2022 Huahang Liu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#include <unistd.h>
#include <zmq.h>

#include <cstdint>
#include <cstdlib>
#include <string>

#include "gflags/gflags.h"
#include "glog/logging.h"

DEFINE_uint32(port, 5555, "port");

namespace {

int64_t GetSystemTimeNanos() {
  return std::chrono::nanoseconds(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

}  // namespace

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  void* context = zmq_ctx_new();
  void* sock = zmq_socket(context, ZMQ_PUB);
  std::string address = "tcp://*:";
  address += std::to_string(FLAGS_port);
  int high_water_mark = 10;
  int rc = zmq_setsockopt(sock, ZMQ_SNDHWM, &high_water_mark,
                          sizeof(high_water_mark));
  CHECK_EQ(rc, 0);
  rc = zmq_bind(sock, address.c_str());
  CHECK_EQ(rc, 0);
  int64_t t0 = GetSystemTimeNanos();
  int i0 = 0;
  std::string data;
  data.resize(1024 * 1024 * 64);
  for (int i = 0;; ++i) {
    {
      std::string data = "/ch1:";
      data += std::to_string(i);
      zmq_send(sock, "ch1\0", 4, ZMQ_SNDMORE);
      zmq_send(sock, data.c_str(), data.size(), 0);
    }
    {
      std::string data = "/ch2:";
      data += std::to_string(i);
      zmq_send(sock, "ch2\0", 4, ZMQ_SNDMORE);
      zmq_send(sock, data.c_str(), data.size() + 1, 0);
    }
    {
      std::string data = "/ch12:";
      data += std::to_string(i);
      zmq_send(sock, "ch12\0", 5, ZMQ_SNDMORE);
      zmq_send(sock, data.c_str(), data.size() + 1, 0);
    }
    int64_t t1 = GetSystemTimeNanos();
    int i1 = i;
    int64_t duration_ns = t1 - t0;
    if (duration_ns > 5000000000) {  // 5 seconds
      double msg_count = static_cast<double>(i1 - i0) * 2.0;
      double messages_per_secs =
          msg_count / (static_cast<double>(duration_ns) / 1000000000.0);
      LOG(INFO) << messages_per_secs << " msgs/s";
      t0 = t1;
      i0 = i1;
    }
    sleep(1);
  }
  return EXIT_SUCCESS;
}
