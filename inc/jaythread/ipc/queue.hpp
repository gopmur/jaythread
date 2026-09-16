#pragma once

#include <optional>
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "portmacro.h"

template <typename T, int N>
class Queue {
  private:
  T queue_data[N];
  StaticQueue_t queue_instance;
  QueueHandle_t queue;

  public:
  Queue();
  bool send(T val, int ticks_to_wait);
  bool send(T val);
  bool send_from_isr(T val);

  std::optional<T> receive(int ticks_to_wait);
  std::optional<T> receive();
  void flush();
  int get_remaining();
  int get_waiting();
};

template <typename T, int N>
Queue<T, N>::Queue() {
  queue = xQueueCreateStatic(N, sizeof(T), reinterpret_cast<uint8_t*>(queue_data), &queue_instance);
}

template <typename T, int N>
bool Queue<T, N>::send(T val, int ticks_to_wait) {
  return xQueueSend(queue, &val, ticks_to_wait);
}

template <typename T, int N>
bool Queue<T, N>::send_from_isr(T val) {
  BaseType_t higher_priority_task_woken = false;
  return xQueueSendFromISR(queue, &val, &higher_priority_task_woken);
}

template <typename T, int N>
bool Queue<T, N>::send(T val) {
  return send(val, portMAX_DELAY);
}

template <typename T, int N>
std::optional<T> Queue<T, N>::receive(int ticks_to_wait) {
  T status;
  auto item_is_available = xQueueReceive(queue, &status, ticks_to_wait);
  if (item_is_available) {
    return status;
  }
  return std::nullopt;
}

template <typename T, int N>
std::optional<T> Queue<T, N>::receive() {
  return receive(portMAX_DELAY);
}

template <typename T, int N>
void Queue<T, N>::flush() {
  xQueueReset(this->queue);
}

template <typename T, int N>
int Queue<T, N>::get_waiting() {
  return uxQueueMessagesWaiting(this->queue);
}

template <typename T, int N>
int Queue<T, N>::get_remaining() {
  return N - uxQueueMessagesWaiting(this->queue);
}