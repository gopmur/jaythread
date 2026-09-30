#pragma once

#include <vector>
#include "freertos/idf_additions.h"
#include "jaythread/ipc/mutex.hpp"

struct ThreadStatus {
  char name[configMAX_TASK_NAME_LEN + 1];
  size_t current_priority;
  size_t base_priority;
  size_t stack_size;
  size_t min_free_stack;
  float cpu_usage;
  eTaskState state;

  const char* get_state_view();
};

struct ThreadRuntime {
  TaskHandle_t handle = nullptr;
  configRUN_TIME_COUNTER_TYPE prev_runtime = 0;
};

class Executable {
  private:
  static Mutex thread_list_mutex;
  static std::vector<Executable> thread_list;
  static std::vector<ThreadRuntime> thread_runtimes;
  static void update_thread_runtimes(std::vector<TaskStatus_t>& status_list);
  size_t stack_size = 0;

  protected:
  TaskHandle_t handle;
  void set_stack_size(size_t stack_size);
  void register_to_list();
  void remove_from_list();

  public:
  void suspend();
  void resume();
  void resume_from_isr();
  void notify();
  void notify_from_isr();

  static std::vector<ThreadStatus> get_threads_status();
};
