#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"

class Cmd_transform
{
public:
	ros::Publisher pub_h;
	ros::Subscriber sub;
	void transform(const std_msgs::String::ConstPtr &command);
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "command_transform_node");
	ros::NodeHandle node;
	Cmd_transform cmd;

	cmd.pub_h = node.advertise<std_msgs::Float64MultiArray>("command_force", 1000);
	cmd.sub = node.subscribe("cmd", 1000, &Cmd_transform::transform, &cmd);
	
	ros::spin();
	return 0;
}

void Cmd_transform::transform(const std_msgs::String::ConstPtr &command)
{
	std_msgs::Float64MultiArray force;
	if (command->data == "stop")
	{
		force.data.push_back(0);
		force.data.push_back(0);
		pub_h.publish(force);
	}
	else if (command->data == "forward")
	{
		force.data.push_back(0.5);
		force.data.push_back(0.5);
		pub_h.publish(force);
	}
	else if (command->data == "backward")
	{
		force.data.push_back(-0.5);
		force.data.push_back(-0.5);
		pub_h.publish(force);
	}
	else if (command->data == "left")
	{
		force.data.push_back(-0.5);
		force.data.push_back(0.5);
		pub_h.publish(force);
	}
	else if (command->data == "right")
	{
		force.data.push_back(0.5);
		force.data.push_back(-0.5);
		pub_h.publish(force);
	}
}

