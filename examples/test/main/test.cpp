#include "esp_log.h"
#include "jaythread/sync.hpp"
#include "jaythread/thread.hpp"

class ConsumerThread : public Thread {
  void main() {
    while (true) {
      Sync::wait_for_notification();
      ESP_LOGI("consumer", "consumer");
    }
  }
};

ConsumerThread consumer_thread;

class ProducerThread : public Thread {
  void main() {
    int i = 0;
    while (true) {
      const int notification_num = 3;
      for (int i = 0; i < notification_num; i++) {
        consumer_thread.notify();
      }
      i++;
      ESP_LOGI("test thread", "%d", i);
      Sync::sleep(1000);
    }
  }
};

ProducerThread producer_thread;

extern "C" void app_main(void) {
  consumer_thread.start("consumer", 1, 4096);
  consumer_thread.start("consumer", 1, 4096);
  producer_thread.start("producer", 2, 4096);
}
