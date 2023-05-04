#pragma once

#include <cstdint>

namespace dk_pull {
namespace event {

enum class EventProvider : uint8_t {
  LIBUV = 1,
  LIBEVENT = 2,
  ASIO = 3,
};

}  // namespace event
}  // namespace dk_pull
