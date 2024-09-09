#include <jetson_uart/jetson_uart.hpp>

#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>
#include <rclcpp_components/register_node_macro.hpp>

#include <serial_connect.hpp>

namespace jetson_uart_node{
	void JetsonToUart::JetsonToUart(const rclcpp::NodeOptions options_) : rclcpp::Node("jetson_uart_node", options_){
		msg_sub = this->create_subscription<can_msgs::msg::Frame>("/can_tx", 10, std::bind(&JetsonToUart::canmsgCallback, this, std::placeholder::_1));
		msg_pub = this->create_publisher<can_msgs::msg::Frame>("/can_rx", 10);

		serial.setSerial("/dev/ttyACM0", 115200);
		serial.openSerial();
		serial.setInterrupt(&JoyToUart::uartCallback);
	}

	void JetsonToUart::~JetsonToUart(void){
		serial.closeSerial();
	}

	void JetsonToUart::canmsgCallback(const can_msgs::msg::Frame::SharedPtr msg_){
		if(msg_->data[2] == 0x22){
			serial.writeSerial(msg_->data, 8);
		}
	}

	void JetsonToUart::uartCallback(int status_){
		if(serial.readSerial()){
			can_msgs::msg::Frame msg;

			msg.header.stamp = this->clock()->now();
			msg.data = serial.recv_data;
			
			msg_pub->publish(msg);
		}
	}
}

RCLCPP_COMPONENTS_REGISTER_NODE(jetson_uart_node::JetsonToUart)