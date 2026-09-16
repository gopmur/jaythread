#include "jaythread/ipc/mutex.hpp"
#include "freertos/idf_additions.h"

Mutex::Mutex() : Mutex(false) {}

Mutex::Mutex(bool recursive) : recursive(recursive) {
  if (recursive) {
    mutex = xSemaphoreCreateRecursiveMutexStatic(&mutex_buffer);
  } else {
    mutex = xSemaphoreCreateMutexStatic(&mutex_buffer);
  }
}

void Mutex::take() {
  if (recursive) {
    xSemaphoreTakeRecursive(this->mutex, portMAX_DELAY);
  } else {
    xSemaphoreTake(this->mutex, portMAX_DELAY);
  }
}

void Mutex::give() {
  if (recursive) {
    xSemaphoreGiveRecursive(this->mutex);
  } else {
    xSemaphoreGive(this->mutex);
  }
}
