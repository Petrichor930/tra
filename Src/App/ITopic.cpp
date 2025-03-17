#include "../Utils/Topic/Topic.hpp"

enum class SensorCategory { RAW_READING, FILTERED_DATA };
class SensorTopic : public Topic<SensorTopic> {
public:
  using TopicCategory = SensorCategory;

  explicit SensorTopic(float val,
                       TopicCategory cat = TopicCategory::RAW_READING)
      : sensor_value(val), category_(cat) {}

  TopicCategory getCategory() const { return category_; }

  float sensor_value;

private:
  TopicCategory category_;
};

// 具体处理器实现示例
class NavigationProcessor : public ITopicHandler<SensorTopic> {
public:
  void handleTopic(const SensorTopic &topic) override {
    if (topic.category() == SensorCategory::FILTERED_DATA) {
      updateTrajectory(topic.sensor_value);
    }
  }

private:
  void updateTrajectory(float filtered_value) {
    // 实现导航逻辑
  }
};

// 系统初始化示例
void initializeSystem() {
  // 配置传感器话题通道
  auto &sensor_channel = TopicRouter::getChannel<SensorTopic>();

  // 添加噪声过滤
  sensor_channel.addFilter(
      std::make_unique<MovingAverageFilter<SensorTopic>>(5));

  // 订阅导航处理器
  sensor_channel.subscribe(std::make_unique<NavigationProcessor>());
}

// 传感器采集任务
void sensorAcquisitionTask(void *) {
  while (true) {
    float raw_value = readSensorData();
    TopicRouter::broadcast<SensorTopic>(raw_value);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}