#pragma once

#include "freertos/idf_additions.h"

class Syncable {
 protected:
  TaskHandle_t handle;

 public:
  void suspend();
  void resume();
  void resume_from_isr();
  void notify();
  void notify_from_isr();
};
