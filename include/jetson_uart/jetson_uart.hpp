#ifndef __JETSON_UART_LIB__
#define __JETSON_UART_LIB__

#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>

#include <serial_connect.hpp>

class JetsonToUart : public rclcpp::Node{
	public:
		explicit JetsonToUart(const rclcpp::NodeOptions options_);
		virtual ~JetsonToUart(void);
	private:
		void canmsgCallback(const can_msgs::msg::Frame::SharedPtr msg_);
		void uartCallback(int status_);

		rclcpp::Subscriber<can_msgs::msg::Frame>::SharedPtr msg_sub;
		rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr msg_pub;

		SerialConnect serial(true);
}

#endif