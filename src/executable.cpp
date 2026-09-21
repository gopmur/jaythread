#include "jaythread/executable.hpp"
#include <cstring>
#include "FreeRTOSConfig.h"
#include "jaythread/consts.hpp"
#include "esp_log.h"
#include "freertos/idf_additions.h"

Mutex Executable::thread_list_mutex;
std::vector<Executable> Executable::thread_list;

void Executable::register_to_list() {
  thread_list_mutex.take();
  for (auto task : thread_list) {
    if (task.handle == this->handle) {
      goto cleanup;
    }
  }
  thread_list.push_back(*this);
cleanup:
  thread_list_mutex.give();
}

void Executable::remove_from_list() {
  thread_list_mutex.take();
  for (int i = 0; i < thread_list.size(); i++) {
    Executable task = thread_list[i];
    if (task.handle == this->handle) {
      thread_list.erase(thread_list.begin() + i);
      break;
    }
  }
  thread_list_mutex.give();
}

void Executable::suspend() {
  vTaskSuspend(handle);
}

void Executable::resume() {
  vTaskResume(handle);
}

void Executable::resume_from_isr() {
  xTaskResumeFromISR(handle);
}

void Executable::notify() {
  xTaskNotifyGive(handle);
}

void Executable::notify_from_isr() {
  BaseType_t higher_priority_task_woken = false;
  vTaskNotifyGiveFromISR(handle, &higher_priority_task_woken);
}

void Executable::set_stack_size(size_t stack_size) {
  this->stack_size = stack_size;
}

std::vector<ThreadStatus> Executable::get_threads_status() {
  size_t native_task_count = uxTaskGetNumberOfTasks();
  std::vector<ThreadStatus> status_list(native_task_count);
  std::vector<TaskStatus_t> native_status_list(native_task_count);
  configRUN_TIME_COUNTER_TYPE total_counter = 0;
  uxTaskGetSystemState(native_status_list.data(), native_task_count,
                       &total_counter);
  if (total_counter == 0) {
    total_counter = 1;
  }
  thread_list_mutex.take();
  auto thread_list = Executable::thread_list;
  thread_list_mutex.give();
  for (size_t i = 0; i < native_task_count; i++) {
    auto status = &status_list[i];
    auto native_status = native_status_list[i];
    uint32_t thread_counter = native_status_list[i].ulRunTimeCounter;
    status->min_free_stack = native_status.usStackHighWaterMark;
    status->current_priority = native_status.uxCurrentPriority;
    status->base_priority = native_status.uxBasePriority;
    status->state = native_status.eCurrentState;
    status->cpu_usage = ((float)thread_counter / (float)total_counter) * 100.0f;
    strncpy(status->name, native_status.pcTaskName, configMAX_TASK_NAME_LEN);
    for (auto thread : thread_list) {
      if (thread.handle == native_status.xHandle) {
        status->stack_size = thread.stack_size;
        break;
      }
    }
  }
  return status_list;
}

const char* ThreadStatus::get_state_view() {
  switch (state) {
    case eRunning:
      return "running";
    case eReady:
      return "ready";
    case eBlocked:
      return "blocked";
    case eSuspended:
      return "suspended";
    case eDeleted:
      return "deleted";
    case eInvalid:
      return "invalid";
  }
  ESP_LOGW(JAY_LOG_TAG, "invalid state %d", state);
  return "unkown";
}