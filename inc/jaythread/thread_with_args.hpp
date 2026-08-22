
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include <string>

template <typename T>
class ThreadWithArg;

template <typename T>
struct ThreadArg {
  ThreadWithArg<T>* self;
  T* arg;
};

template <typename T>
class ThreadWithArg {
 protected:
  TaskHandle_t handle = nullptr;

 public:
  void start(std::string name, int priority, int stack_size, T arg);
  static void _main(ThreadArg<T>* arg);
  virtual void main(T*) = 0;
};

template <typename T>
void ThreadWithArg<T>::_main(ThreadArg<T>* _arg) {
  ThreadWithArg<T>* self = _arg->self;
  T* arg = _arg->arg;
  self->main(arg);
  delete arg;
  self->handle = nullptr;
  vTaskDelete(nullptr);
}

template <typename T>
void ThreadWithArg<T>::start(std::string name,
                             int priority,
                             int stack_size,
                             T arg) {
  if (handle != nullptr) {
    ESP_LOGE(__FILE__, "duplicate start on thread");
    fflush(stdout);
    abort();
    return;
  }

  auto thread_arg = new ThreadArg<T>;
  thread_arg->arg = new T(arg);
  thread_arg->self = this;
  xTaskCreate(reinterpret_cast<void (*)(void*)>(_main), name.c_str(),
              stack_size, thread_arg, priority, &this->handle);
}