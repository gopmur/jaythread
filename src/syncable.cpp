#include "jaythread/syncable.hpp"
#include "freertos/idf_additions.h"

void Syncable::suspend() {
  vTaskSuspend(handle);
}

void Syncable::resume() {
  vTaskResume(handle);
}

void Syncable::resume_from_isr() {
  xTaskResumeFromISR(handle);
}

void Syncable::notify() {
  xTaskNotifyGive(handle);
}

void Syncable::notify_from_isr() {
  BaseType_t higher_priority_task_woken = false;
  vTaskNotifyGiveFromISR(handle, &higher_priority_task_woken);
}