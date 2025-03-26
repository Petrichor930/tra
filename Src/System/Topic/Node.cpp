// #include "Node.hpp"
//
// template <typename TxType, typename RxType> class Node {
// public:
//   // subscribe method
//   void Subscribe(Node<RxType, TxType> *sub) {
//     xSemaphoreTake(mutex_, portMAX_DELAY);
//     subscribers.push_back(sub);
//     xSemaphoreGive(mutex_);
//
//     // forward task creation
//     // xTaskCreate(forwardTask, "MultiForward", 512, this, 2, nullptr);
//   }
//
//   // publish method
//   BaseType_t Publish(const TxType &data, TickType_t timeout = portMAX_DELAY) {
//     BaseType_t res = pdFALSE;
//     if (xSemaphoreTake(mutex_, timeout) == pdTRUE) {
//       if (xQueueSend(tx_queue_, &data, 0) == pdPASS) {
//         res = pdTRUE;
//       }
//       // boardcast to all subscribers
//       for (auto &sub : subscribers_) {
//         xQueueSend(sub->rx_queue_, &data, 0);
//       }
//       xSemaphoreGive(mutex);
//     }
//     return res;
//   }
//
// private:
//   std::vector<Node<RxType, TxType> *> subscribers_;
//
//   // take Node's tx_queue buffer
//   // and boardcast the data to all subscribers
//   static void forwardTask(void *params) {
//     auto *self = static_cast<Node *>(params);
//     TxType data;
//
//     while (1) {
//       if (xQueueReceive(self->tx_queue_, &data, portMAX_DELAY) == pdPASS) {
//         xSemaphoreTake(self->mutex_, portMAX_DELAY);
//         for (auto &sub : self->subscribers_) {
//           xQueueSend(sub->rx_queue_, &data, 0);
//         }
//         xSemaphoreGive(self->mutex_);
//       }
//     }
//   }
// };
