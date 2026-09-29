#include "jaythread/timer.hpp"
#include <string.h>
#include "FreeRTOSConfig.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "jaythread/consts.hpp"
#include "portmacro.h"

void Timer::init(const char* name, uint32_t period_ms, bool auto_reload) {
  if (initialized.exchange(true)) {
    ESP_LOGW(JAY_LOG_TAG, "timer is already initialized");
  }
  strncpy(this->name, name, configMAX_TASK_NAME_LEN);
  this->name[configMAX_TASK_NAME_LEN] = '\0';
  handle = xTimerCreate(this->name, pdMS_TO_TICKS(period_ms), auto_reload, this,
                        _main);
}

void Timer::_main(TimerHandle_t handle) {
  auto self = static_cast<Timer*>(pvTimerGetTimerID(handle));
  if (self == nullptr) {
    ESP_LOGW(JAY_LOG_TAG, "timer callback called with null this pointer");
    return;
  }
  self->main();
  self->started = false;
}

void Timer::start(uint32_t ms_to_wait) {
  if (!initialized) {
    ESP_LOGW(JAY_LOG_TAG, "timer start called on uninitialized timer");
    return;
  }
  if (started.exchange(true)) {
    return;
  }
  xTimerStart(handle, pdMS_TO_TICKS(ms_to_wait));
}

void Timer::stop(uint32_t ms_to_wait) {
  if (!initialized) {
    ESP_LOGW(JAY_LOG_TAG, "timer stop called on uninitialized timer");
    return;
  }
  xTimerStop(handle, pdMS_TO_TICKS(ms_to_wait));
  started = false;
}

void Timer::start_block() {
  if (!initialized) {
    ESP_LOGW(JAY_LOG_TAG, "timer start called on uninitialized timer");
    return;
  }
  if (started.exchange(true)) {
    return;
  }
  xTimerStart(handle, portMAX_DELAY);
}

void Timer::stop_block() {
  if (!initialized) {
    ESP_LOGW(JAY_LOG_TAG, "timer stop called on uninitialized timer");
    return;
  }
  xTimerStop(handle, portMAX_DELAY);
  started = false;
}