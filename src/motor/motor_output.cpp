#include <vector>
#include <cmath>
#include <unistd.h>
#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include "pca9685_output/pca9685_output.h"
#include "motor_graph.h"

const double length = 0.8;
const double width = 0.6;

// transfer moment and force to the magnitude of each motor force
const double trans_mtx[4][4] = {
	{1/(2*width),-1/(2*length),0.25,0.25},
	{1/(2*width),1/(2*length),-0.25,0.25},
	{-1/(2*width),1/(2*length),0.25,0.25},
	{-1/(2*width),-1/(2*length),-0.25,0.25}};

class Motor_output
{
private:
	int xxx;
	double motor_force[6];
	int output_signal[6];
	PCA9685_output *motor_signal;
	ros::Subscriber sub;
	ros::Subscriber sub_h;

public:
	Motor_output();
	~Motor_output();
	ros::Subscriber& subscriber(){return sub;}
	ros::Subscriber& subscriber_h(){return sub_h;}
	void transfer(std::vector<double> f);
	int transform(const double m);
	void output(const std_msgs::Float64MultiArray::ConstPtr &input_force);
	void output_h(const std_msgs::Float64MultiArray::ConstPtr &input_force);
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "motor_output_node");
	ros::NodeHandle node;

	Motor_output motor_output;
	motor_output.subscriber() = node.subscribe("balance_force", 1000, &Motor_output::output, &motor_output);
	motor_output.subscriber_h() = node.subscribe("command_force", 1000, &Motor_output::output_h, &motor_output);

	ros::spin();
	return 0;
}

Motor_output::Motor_output()
{
	xxx = 0;
	for (int i = 0; i < 4; i++)
	{
		motor_force[i] = 0;
		output_signal[i] = 0;
	}
	motor_signal = new PCA9685_output(233.5);
	sleep(1);
}

Motor_output::~Motor_output()
{
	delete motor_signal;
}

void Motor_output::transfer(std::vector<double> f)
{
	for(int i = 0;i < 4;i++) // matrix multiplication
	{
		double tmp = 0;
		for(int j =  0;j < 4;j++)
		{
			tmp  += trans_mtx[i][j] * f[j];
		}
		motor_force[i] = tmp;
	}
}

int Motor_output::transform(const double m)
{
	// searching for the closest value from the above force-signal table
        for (int j = 0; j < 201; j++)
       	{
		if(FORCE[j] > m)
      		{
               		if(abs(FORCE[j-1] - m) < abs(FORCE[j] - m))
              			return SIGNAL[j - 1];
               		else
              			return SIGNAL[j];
      		}
      	}
	return SIGNAL[200];
 }

void Motor_output::output(const std_msgs::Float64MultiArray::ConstPtr &input_force)
{
	motor_force[0] = -input_force->data[0] + input_force->data[1] + input_force->data[3];
	motor_force[1] = -(-input_force->data[0] - input_force->data[1] + input_force->data[3]);
	motor_force[2] = -(input_force->data[0] + input_force->data[1] + input_force->data[3]);
	motor_force[3] = input_force->data[0] - input_force->data[1] + input_force->data[3];


/*	std::cout << motor_force[0] << "  ";
	std::cout << motor_force[1] << "  ";
	std::cout << motor_force[2] << "  ";
	std::cout << motor_force[3] << "  \n";*/

	for (int i = 0; i < 4; i++)
	{
//		std::cout << motor_force[i] << "  ";
		std::cout << transform(motor_force[i]) << "  ";
		motor_signal->set_PWM_OFF(i,transform(motor_force[i]));
	}
	std::cout << std::endl;
}

void Motor_output::output_h(const std_msgs::Float64MultiArray::ConstPtr &input_force)
{
	motor_signal->set_PWM_OFF(4, transform(-(input_force->data.at(0))));
	motor_signal->set_PWM_OFF(5, transform(input_force->data.at(1)));
}

/*void Motor_output::output(const std_msgs::Float64MultiArray::ConstPtr &input_force)
{
	transform(input_force->data
}*/
