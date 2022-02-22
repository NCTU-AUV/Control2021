#ifndef PID_PUBLISHER
#define PID_PUBLISHER

#include <vector>
#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include "pid_controller/pid_controller.h"

class PID_publisher
{
private:
	ros::Publisher pub;
	ros::Subscriber sub;

	std::vector<PID_attitude_controller> attitude_controller;
	PID_depth_controller *depth_controller;
public:
	PID_publisher(const double k[][3]);
	ros::Publisher& publisher();
	ros::Subscriber& subscriber();
	void publish(const std_msgs::Float64MultiArray::ConstPtr &error);
};

#endif
