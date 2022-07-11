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

#include <zmq.h>

#include <cstdlib>
#include <string>
#include <vector>

#include "dk_pull/common/finally.h"
#include "dk_pull/common/split_string.h"
#include "gflags/gflags.h"
#include "glog/logging.h"

DEFINE_string(address, "tcp://localhost:5555", "address");

DEFINE_string(channels, "ch1,ch2", "channels");

int main(int argc, char** argv) {
  using dk_pull::common::Finally;
  using dk_pull::common::SplitString;
  google::ParseCommandLineFlags(&argc, &argv, true);
  void* context = zmq_ctx_new();
  CHECK_NE(context, nullptr);
  void* poller = zmq_poller_new();
  CHECK_NE(poller, nullptr);
  Finally finally([&context, &poller]() {
    CHECK_EQ(zmq_poller_destroy(&poller), 0);
    CHECK_EQ(zmq_ctx_term(context), 0);
  });
  std::vector<std::string> channels;
  SplitString(&channels, FLAGS_channels, ',');
  for (auto& ch : channels) {
    void* sock = zmq_socket(context, ZMQ_SUB);
    CHECK_NE(sock, nullptr);
    int rc = zmq_connect(sock, FLAGS_address.c_str());
    CHECK_EQ(rc, 0);
    ch.push_back('\0');
    rc = zmq_setsockopt(sock, ZMQ_SUBSCRIBE, ch.c_str(), ch.size());
    CHECK_EQ(rc, 0);
    int high_water_mark = 5;
    rc = zmq_setsockopt(sock, ZMQ_RCVHWM, &high_water_mark,
                        sizeof(high_water_mark));
    CHECK_EQ(rc, 0);
    rc = zmq_poller_add(poller, sock, nullptr, ZMQ_POLLIN);
    CHECK_EQ(rc, 0);
  }
  zmq_poller_event_t events[1];
  zmq_msg_t msg_channel;
  zmq_msg_t msg_body;
  while (true) {
    int rc = zmq_poller_wait_all(poller, events, 1, -1);
    CHECK_GT(rc, 0);
    for (int i = 0; i < rc; ++i) {
      CHECK_NE((events[i].events & ZMQ_POLLIN), 0);
      zmq_msg_init(&msg_channel);
      zmq_msg_init(&msg_body);
      zmq_msg_recv(&msg_channel, events[i].socket, 0);
      CHECK_EQ(zmq_msg_more(&msg_channel), 1);
      zmq_msg_recv(&msg_body, events[i].socket, 0);
      CHECK_EQ(zmq_msg_more(&msg_body), 0);
      LOG(INFO) << "channel: "
                << reinterpret_cast<const char*>(zmq_msg_data(&msg_channel))
                << ", "
                << "body bytes: " << zmq_msg_size(&msg_body) << ", "
                << "body: "
                << reinterpret_cast<const char*>(zmq_msg_data(&msg_body));
      zmq_msg_close(&msg_channel);
      zmq_msg_close(&msg_body);
    }
  }
  return EXIT_SUCCESS;
}
