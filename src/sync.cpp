#include "jaythread/sync.hpp"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

void Sync::sleep(size_t delay_ms) {
  vTaskDelay(pdMS_TO_TICKS(delay_ms));
}

void Sync::wait_for_notification_and_clear() {
  ulTaskNotifyTake(true, portMAX_DELAY);
}

bool Sync::wait_for_notification_and_clear(int ms_to_wait) {
  return ulTaskNotifyTake(true, pdMS_TO_TICKS(ms_to_wait)) != 0;
}

void Sync::wait_for_notification() {
  ulTaskNotifyTake(false, portMAX_DELAY);
}

bool Sync::wait_for_notification(int ms_to_wait) {
  return ulTaskNotifyTake(false, pdMS_TO_TICKS(ms_to_wait)) != 0;
}