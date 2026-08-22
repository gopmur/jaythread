#include "jaythread/thread.hpp"
#include "freertos/idf_additions.h"

void Thread::_main(Thread* self) {
  self->main();
}

void Thread::start(std::string name, int priority, int stack_size) {
  xTaskCreate(reinterpret_cast<void (*)(void*)>(_main), name.c_str(),
              stack_size, this, priority, &this->handle);
}