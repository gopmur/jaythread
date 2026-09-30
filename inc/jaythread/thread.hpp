#pragma once

#include <atomic>
#include <string>
#include "jaythread/executable.hpp"
#include "executable.hpp"

class Thread : public Executable {
 private:
  std::atomic_bool started = false;

 public:
  bool start(std::string name, int priority, int stack_size);
  static void _main(Thread* self);
  virtual void main() = 0;
};
