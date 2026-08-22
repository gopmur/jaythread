#include "esp_log.h"
#include "jaythread/sync.hpp"
#include "jaythread/thread.hpp"



class ConsumerThread : public Thread {
  void main() {
    
  }
};

ConsumerThread consumer_thread;

class ProducerThread : public Thread {
  void main() {
    const int notification_num = 3;
    for (int i = 0; i < notification_num; i++) {
      
    }
    while (true) {
      i++;
      ESP_LOGI("test thread", "%d", i);
      Sync::sleep(1000);
    }
  }
};

TestThread test_thread;

extern "C" void app_main(void) {
  test_thread.start("test_thread", 1, 4096);
}
