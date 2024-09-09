cmake_minimum_required(VERSION 3.8)
project(jetson_uart)

set(jetson_uart_MAJOR_VERSION 0)
set(jetson_uart_MINOR_VERSION 0)
set(jetson_uart_PATCH_VERSION 0)
set(jetson_uart_VERSION
  ${jetson_uart_MAJOR_VERSION}.${jetson_uart_MINOR_VERSION}.${jetson_uart_PATCH_VERSION})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(amen_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

ament_auto_add_library(jetson_uart src/jetson_uart.cpp)
rclcpp_components_register_nodes(jetson_uart "jetson_uart_node::JetsonToUart")

ament_auto_package()