#include "pid_publisher.h"

//order of k value: roll(x), pitch(y), yaw(z), depth(z)
//order: kp, pi, kd
/*double k[4][3] = {
	{0.011, 0, 0},
	{0.01, 0, 0},
	{0, 0, 0},
	{0.1, 0, 0.1}};*/

double k[4][3] = {
	{0.01, 0, 0},
	{0.01, 0, 0},
	{0, 0, 0},
	{0, 0, 0}};

int main(int argc, char **argv)
{
	//init ros node
	ros::init(argc, argv, "pid_controller");
	ros::NodeHandle node;
	//init pid_publisher class and set k value
	PID_publisher pid_publisher(k);
	//std::cout << "Yes!" << std::endl;

	//init publisher and subscriber
	pid_publisher.publisher() = node.advertise<std_msgs::Float64MultiArray>("balance_force", 1000);
	pid_publisher.subscriber() =  node.subscribe("sensor_input_error", 1000, &PID_publisher::publish, &pid_publisher);
	
	ros::spin();
	return 0;
}

