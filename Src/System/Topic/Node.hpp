// #pragma once
//
// #include "FreeRTOS.h"
// #include "portmacro.h"
// #include "queue.h"
// #include "semphr.h"
// #include "task.h"
//
// #include <vector>
//
// template <typename TxType, typename RxType> class Node {
// public:
//     void Subscribe(Node<RxType, TxType> *sub);
//
//     BaseType_t Publish(const TxType &data, TickType_t timeout = portMAX_DELAY);
//
// private:
//     // subscriber list of this Node
//     std::vector<Node<RxType, TxType> *> subscribers_;
//
//     static void forwardTask(void *params);
//
//     SemaphoreHandle_t mutex_;
//
//     // This Node's Rx Buffer Queue
//     QueueHandle_t rx_queue_;
//
//     // This Node's Tx Buffer Queue
//     QueueHandle_t tx_queue_;
// };
