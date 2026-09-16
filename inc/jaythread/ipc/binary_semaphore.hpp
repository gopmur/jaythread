#pragma once

#include "freertos/idf_additions.h"

class BinarySemaphore {
  private:
  SemaphoreHandle_t semaphore;
  StaticSemaphore_t semaphore_buffer;

  public:
  BinarySemaphore();
  BinarySemaphore(bool recursive);

  void take();
  void give();
  void clear();
};