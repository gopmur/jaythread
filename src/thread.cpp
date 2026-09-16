#include "jaythread/thread.hpp"
#include "jaythread/consts.hpp"
#include "esp_log.h"
#include "freertos/idf_additions.h"

void Thread::_main(Thread* self) {
  self->main();
  self->started = false;
  vTaskDelete(nullptr);
}

void Thread::start(std::string name, int priority, int stack_size) {
  if (started.exchange(true)) {
    ESP_LOGE(JAY_LOG_TAG, "duplicate start called on thread %s", name.c_str());
    return;
  }
  xTaskCreate(reinterpret_cast<void (*)(void*)>(_main), name.c_str(),
              stack_size, this, priority, &this->handle);
}