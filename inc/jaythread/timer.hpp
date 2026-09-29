#pragma once

#include <atomic>
#include "FreeRTOSConfig.h"
#include "freertos/idf_additions.h"

class Timer {
  private:
  TimerHandle_t handle;
  std::atomic<bool> initialized = false;
  std::atomic<bool> started = false;
  char name[configMAX_TASK_NAME_LEN + 1];
  static void _main(TimerHandle_t handle);

  public:
  virtual void main() = 0;
  void init(const char* name, uint32_t period_ms, bool auto_reload);
  void start(uint32_t ms_to_wait);
  void stop(uint32_t ms_to_wait);
  void start_block();
  void stop_block();
};