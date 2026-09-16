#pragma once

#include "freertos/idf_additions.h"

class Mutex {
  private:
  SemaphoreHandle_t mutex;
  StaticSemaphore_t mutex_buffer;
  bool recursive;

  public:
  Mutex();
  Mutex(bool recursive);

  void take();
  void give();
};