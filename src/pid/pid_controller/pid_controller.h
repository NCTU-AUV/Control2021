#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PID_controller
{
protected:
	double kp;
	double ki;
	double kd;
	double error_sum;

public:
	PID_controller(double p, double i, double d);
	void set_kp(double p){kp = p;}
	void set_ki(double i){ki = i;}
	void set_kd(double d){kd = d;}
	double get_kp(){return kp;}
	double get_ki(){return ki;}
	double get_kd(){return kd;}
};

class PID_attitude_controller:public PID_controller
{
public:
	PID_attitude_controller(double p, double i, double d);
	double output_force_calculation(double e_angle, double e_angular_velocity);
};

class PID_depth_controller:public PID_controller
{
private:
	double pre_error;

public:
	PID_depth_controller(double p, double i, double d);
	double output_force_calculation(double e);
};
#endif
