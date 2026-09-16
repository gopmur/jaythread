#pragma once

#include <atomic>
#include <string>
#include "syncable.hpp"

class Thread : public Syncable {
 private:
  std::atomic_bool started = false;

 public:
  void start(std::string name, int priority, int stack_size);
  static void _main(Thread* self);
  virtual void main() = 0;
};
