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
  bool start(std::string name, int priority);
  static void _main(StaticThread<STACK_SIZE>* self);
  virtual void main() = 0;
};

template <size_t STACK_SIZE>
bool StaticThread<STACK_SIZE>::start(std::string name, int priority) {
  if (handle != nullptr) {
    return false;
  }
  set_stack_size(STACK_SIZE);
  handle = xTaskCreateStatic(_main, name.c_str(), STACK_SIZE, nullptr, priority,
                             stack, &task_buffer);
  this->register_to_list();
  return false;
}