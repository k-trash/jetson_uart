#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>

#include <serial_connect/serial_connect.hpp>

rclcpp::Node::SharedPtr node;
rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr msg_sub;
rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr msg_pub;

SerialConnect serial;

void canmsgCallback(const can_msgs::msg::Frame::SharedPtr msg_){
	uint8_t send_data[8] = {0u};
	if(msg_->id == 0x7D7){
		for(uint8_t i=0; i<8; i++){
			send_data[i] = msg_->data[i];
		}

		serial.writeSerial(send_data, 8);
	}
}

void uartCallback(int status_){
	if(serial.readSerial()){
		can_msgs::msg::Frame msg;

		msg.header.stamp = node->get_clock()->now();
		for(uint8_t i=0; i<8; i++){
			msg.data[i] = serial.recv_data[i];
		}
		msg_pub->publish(msg);
	}
}

int main(int argc, char *argv[]){
	rclcpp::init(argc, argv);
	node = rclcpp::Node::make_shared("throttle_node");

	msg_sub = node->create_subscription<can_msgs::msg::Frame>("/can_tx", 10, &canmsgCallback);
	msg_pub = node->create_publisher<can_msgs::msg::Frame>("/from_can_bus", 10);

	serial.setSerial("/dev/ttyTHS0", B115200, true);
	serial.openSerial();

	serial.setInterrupt(&uartCallback);

	rclcpp::spin(node);

	rclcpp::shutdown();

	serial.closeSerial();

	return 0;	
}