#include "jaythread/thread.hpp"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "jaythread/consts.hpp"

void Thread::_main(Thread* self) {
  self->main();
  self->remove_from_list();
  self->started = false;
  vTaskDelete(nullptr);
}

bool Thread::start(std::string name, int priority, int stack_size) {
  if (started.exchange(true)) {
    ESP_LOGE(JAY_LOG_TAG, "duplicate start called on thread %s", name.c_str());
    return false;
  }
  set_stack_size(stack_size);
  xTaskCreate(reinterpret_cast<void (*)(void*)>(_main), name.c_str(),
              stack_size, this, priority, &this->handle);
  register_to_list();
  return false
}