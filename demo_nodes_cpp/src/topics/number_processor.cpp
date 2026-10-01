#include <memory>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "std_msgs/msg/string.hpp"

#include "demo_nodes_cpp/visibility_control.h"

namespace demo_nodes_cpp
{
class NumberProcessor : public rclcpp::Node
{
public:
  DEMO_NODES_CPP_PUBLIC
  explicit NumberProcessor(const rclcpp::NodeOptions & options)
  : Node("number_processor", options)
  {
    // Publisher: publica el resultado en /processed_data
    pub_ = create_publisher<std_msgs::msg::String>("processed_data", 10);

    // Callback: se ejecuta cada vez que llega un mensaje a /chatter
    auto callback =
      [this](std_msgs::msg::String::ConstSharedPtr msg) -> void
      {
        // El mensaje es "Fibonacci: 13": nos quedamos con lo que hay tras ':'
        auto pos = msg->data.find(':');
        if (pos == std::string::npos) {
          RCLCPP_WARN(get_logger(), "Formato inesperado: '%s'", msg->data.c_str());
          return;
        }

        try {
          long long n = std::stoll(msg->data.substr(pos + 1));
          long long result = n * n;

          RCLCPP_INFO(get_logger(), "%lld² = %lld", n, result);

          std_msgs::msg::String out;
          out.data = "Result: " + std::to_string(result);
          pub_->publish(out);
        } catch (const std::exception & e) {
          RCLCPP_WARN(get_logger(), "No pude convertir a número: '%s'", msg->data.c_str());
        }
      };

    // Subscriber: escucha en /chatter
    sub_ = create_subscription<std_msgs::msg::String>("chatter", 10, callback);
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
};
}  // namespace demo_nodes_cpp

RCLCPP_COMPONENTS_REGISTER_NODE(demo_nodes_cpp::NumberProcessor)
