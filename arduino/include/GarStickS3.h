#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>

// Platform I/O only. No application protocol, sessions, UI or answer semantics.
namespace gar::sticks3 {
enum class Link { usb };
struct Hardware {
  bool powered=true;
  int battery=0, width=0, height=0, board=0;
  bool charging=false;
};
struct Options {
  uint8_t rotation, brightness;
};
struct Hooks {
  std::function<void(Link,const char*,size_t)> receive;
  std::function<void(Link,bool)> reset;  // true: discard a broken byte stream
  std::function<void(char,int)> button;
  std::function<void(const Hardware&)> hardware;
};
void begin(const Options& options,Hooks hooks);
void setBrightness(uint8_t brightness);
void poll();  // Owns the LVGL clock and hardware callbacks on the main task.
Hardware hardware();
void send(Link link,const std::string& bytes);
}
