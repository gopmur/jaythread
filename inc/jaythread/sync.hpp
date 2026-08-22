#pragma once

#include <cstddef>

class Sync {
 public:
  static void wait_for_notification();
  static bool wait_for_notification(int ticks_to_wait);
  static void wait_for_notification_and_clear();
  static bool wait_for_notification_and_clear(int ticks_to_wait);
  static void sleep(size_t delay_ms);
};