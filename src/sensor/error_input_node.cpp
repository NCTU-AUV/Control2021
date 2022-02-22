#include "serial_reader/Serial_reader.h"
#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/String.h"
#include <iostream>

const int data_n = 7;
double set_point[data_n] = {1.14, 0.97, 0, -0.07, 0.1, 0, 202};

void change_sp(const std_msgs::String::ConstPtr &command);

int main(int argc, char **argv)
{
	std_msgs::Float64MultiArray error_input;
	ros::init(argc, argv, "sensor_error");
	ros::NodeHandle node;
	ros::Publisher pub = node.advertise<std_msgs::Float64MultiArray>("sensor_input_error", 1000);

	ros::Subscriber sub = node.subscribe("cmd", 1000, change_sp);

	Serial_reader imu_reader("/dev/ttyACM1", 115200, data_n);

	std::cout << "begin reading data\n";
	while (ros::ok())
	{
		ros::spinOnce();
		error_input.data.clear();
		std::vector<float> data = imu_reader.read_float_data();
		for (int i = 0; i < data_n; i++)
			error_input.data.push_back(set_point[i] - (double)data.at(i));
		pub.publish(error_input);
	}
	return 0;
}

void change_sp(const std_msgs::String::ConstPtr &command)
{
	if (command->data == "up")
		set_point[6] -= 0.1;
	else if (command->data == "down")
		set_point[6] += 0.1;
}
