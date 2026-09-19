#pragma once

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "jaythread/executable.hpp"

template <size_t STACK_SIZE>
class StaticThread : public Executable {
 private:
  uint8_t stack[STACK_SIZE];
  StaticTask_t task_buffer;
  bool instantiated = false;

 public:
  void start(std::string name, int priority);
  static void _main(StaticThread<STACK_SIZE>* self);
  virtual void main() = 0;
};

template <size_t STACK_SIZE>
void StaticThread<STACK_SIZE>::start(std::string name, int priority) {
  if (handle != nullptr) {
    return;
  }
  set_stack_size(STACK_SIZE);
  handle = xTaskCreateStatic(_main, name.c_str(), STACK_SIZE, nullptr, priority,
                             stack, &task_buffer);
  this->register_to_list();
}