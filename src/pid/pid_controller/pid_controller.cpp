#include "pid_controller.h"

PID_controller::PID_controller(double p, double i, double d)
{
	kp = p;
	ki = i;
	kd = d;
	error_sum = 0.0;
}

PID_attitude_controller::PID_attitude_controller(double p, double i, double d):PID_controller(p, i, d)
{
}

double PID_attitude_controller::output_force_calculation(double e_angle, double e_angular_velocity)
{
	error_sum += e_angle;
	return (kp * e_angle + ki * error_sum - kd * e_angular_velocity);
}

PID_depth_controller::PID_depth_controller(double p, double i, double d):PID_controller(p, i, d)
{
	pre_error = 0;
}

double PID_depth_controller::output_force_calculation(double e)
{
	double output = kp * e + ki * error_sum + kd * (e - pre_error);
	error_sum += e;
	pre_error = e;
	return output;
}
