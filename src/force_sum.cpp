#include <vector>
#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"

class Force_publisher
{
private:
	std_msgs::Float64MultiArray sum;
	std::vector<double> balance_force;
	std::vector<double> command_force;
	
	ros::Publisher pub;
	ros::Subscriber sub[2];

public:
	Force_publisher();
	ros::Publisher& publisher(){return pub;}
	ros::Subscriber& subscriber(int i){return sub[i];}
	void add_balance_force(const std_msgs::Float64MultiArray::ConstPtr &b_force);
	void add_command_force(const std_msgs::Float64MultiArray::ConstPtr &c_force);
	void publish_force();
};

int main(int argc, char **argv)
{
	Force_publisher sum_f;

	ros::init(argc, argv, "force_summation_node");
	ros::NodeHandle node;

	sum_f.publisher() = node.advertise<std_msgs::Float64MultiArray>("Motor_Force", 1000);
	sum_f.subscriber(0) = node.subscribe("balance_force", 1000, &Force_publisher::add_balance_force, &sum_f);
//	sum_f.subscriber(1) = node.subscribe("command_force", 1000, &Force_publisher::add_command_force, &sum_f);

	ros::spin();
	return 0;
}

Force_publisher::Force_publisher()
{
	for (int i = 0; i < 4; i++)
	{
		sum.data.push_back(0);
		balance_force.push_back(0);
		command_force.push_back(0);
	}
}

void Force_publisher::add_balance_force(const std_msgs::Float64MultiArray::ConstPtr &b_force)
{
	for (int i = 0; i < 4; i++)
		balance_force.at(i) = b_force->data.at(i);
	publish_force();
}

void Force_publisher::add_command_force(const std_msgs::Float64MultiArray::ConstPtr &c_force)
{
	for (int i = 0; i < 4; i++)
		command_force.at(i) = c_force->data.at(i);
	publish_force();
}

void Force_publisher::publish_force()
{
	for (int i = 0; i < 4; i++)
		sum.data.at(i) = balance_force.at(i) + command_force.at(i);
	pub.publish(sum);
}
