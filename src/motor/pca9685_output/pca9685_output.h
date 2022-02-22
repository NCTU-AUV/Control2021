#ifndef PCA9685_OUTPUT_H
#define PCA9685_OUTPUT_H

#include <wiringPiI2C.h>

const int address = 0x40;

class PCA9685_output
{
private:
	int fd;

public:
	PCA9685_output(const double f);
	~PCA9685_output();
	int set_sleep();
	int unset_sleep();
	void set_PWM_ON(const int ch, const unsigned short int value);
	void set_PWM_OFF(const int ch, unsigned short int value);

};

#endif
