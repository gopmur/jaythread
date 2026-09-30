#pragma once

#include <atomic>
#include <string>
#include "consts.hpp"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "jaythread/executable.hpp"
#include "jaythread/sync.hpp"

template <typename T>
class ThreadWithArg;

template <typename T>
struct ThreadArg {
  ThreadWithArg<T>* self;
  T* arg;
};

template <typename T>
class ThreadWithArg : public Executable {
 private:
  std::atomic_bool started = false;

 public:
  bool start(std::string name, int priority, int stack_size, T arg);
  static void _main(ThreadArg<T>* arg);
  virtual void main(T*) = 0;
};

template <typename T>
void ThreadWithArg<T>::_main(ThreadArg<T>* _arg) {
  ThreadWithArg<T>* self = _arg->self;
  T* arg = _arg->arg;
  self->main(arg);
  self->remove_from_list();
  delete arg;
  self->handle = nullptr;
  self->started = false;
  vTaskDelete(nullptr);
}

template <typename T>
bool ThreadWithArg<T>::start(std::string name,
                             int priority,
                             int stack_size,
                             T arg) {
  if (started.exchange(true)) {
    ESP_LOGE(JAY_LOG_TAG, "duplicate start called on thread %s", name.c_str());
    return false;
  }
  set_stack_size(stack_size);
  auto thread_arg = new ThreadArg<T>;
  thread_arg->arg = new T(arg);
  thread_arg->self = this;
  xTaskCreate(reinterpret_cast<void (*)(void*)>(_main), name.c_str(),
              stack_size, thread_arg, priority, &this->handle);
  register_to_list();
  return true;
}