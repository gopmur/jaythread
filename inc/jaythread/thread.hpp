#pragma once

#include <string>
#include "syncable.hpp"

class Thread : protected Syncable {
 protected:
  TaskHandle_t handle;

 public:
  void start(std::string name, int priority, int stack_size);
  static void _main(Thread* self);
  virtual void main() = 0;
};
