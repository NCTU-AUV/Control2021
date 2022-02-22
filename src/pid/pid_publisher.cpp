#include "pid_publisher.h"

PID_publisher::PID_publisher(const double k[][3])
{
	for (int i = 0; i < 3; i++)
		attitude_controller.push_back(PID_attitude_controller(k[i][0], k[i][1], k[i][2]));
	depth_controller = new PID_depth_controller(k[3][0], k[3][1], k[3][2]);
}

ros::Publisher& PID_publisher::publisher()
{
	return pub;
}

ros::Subscriber& PID_publisher::subscriber()
{
	return sub;
}

void PID_publisher::publish(const std_msgs::Float64MultiArray::ConstPtr &error)
{
	std_msgs::Float64MultiArray force_matrix;
	float roll, pitch;

	force_matrix.data.push_back(attitude_controller.at(0).output_force_calculation(error->data[0], error->data[3]));
	force_matrix.data.push_back(attitude_controller.at(1).output_force_calculation(error->data[1], error->data[4]));
	force_matrix.data.push_back(attitude_controller.at(2).output_force_calculation(error->data[2], error->data[5]));
	force_matrix.data.push_back(depth_controller->output_force_calculation(error->data[6]) + 0.6);

//	force_matrix.data.push_back(0.6);

/*	roll = attitude_controller.at(0).output_force_calculation(error->data[0], error->data[3]);
	pitch = attitude_controller.at(1).output_force_calculation(error->data[1], error->data[4]);

	force_matrix.data.push_back(roll - pitch);
       	force_matrix.data.push_back(roll + pitch);
        force_matrix.data.push_back(-roll - pitch);
       	force_matrix.data.push_back(-roll + pitch);*/
	pub.publish(force_matrix);
	
/*	for (int i = 0; i < 4; i++)
		std::cout << force_matrix.data[i] << " ";
	std::cout << std::endl;*/
}

