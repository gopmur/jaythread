#pragma once

#include <string>
#include "syncable.hpp"

class Thread : public Syncable {
 public:
  void start(std::string name, int priority, int stack_size);
  static void _main(Thread* self);
  virtual void main() = 0;
};
