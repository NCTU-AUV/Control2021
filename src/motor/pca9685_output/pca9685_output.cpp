#include "pca9685_output.h"
#include <iostream>

PCA9685_output::PCA9685_output(const double f = 233.5)
{
	fd = wiringPiI2CSetup(address);
	std::cout << fd << std::endl;
	while (set_sleep() < 0);
	while (wiringPiI2CWriteReg8(fd, 0xFE, (int)((25000000 / (4096 * f)) - 1)) < 0);
	for (int i = 0; i < 16; i++)
	{
		set_PWM_ON(i, 0);
		set_PWM_OFF(i, 1500);
	}
	unset_sleep();
	std::cout << "set up\n";
}


PCA9685_output::~PCA9685_output()
{
	for (int i = 0; i < 16; i++)
		set_PWM_OFF(i, 1500);
	while (set_sleep() < 0);
}

int PCA9685_output::set_sleep()
{
	int old_value = wiringPiI2CReadReg8(fd, 0x00);
	return wiringPiI2CWriteReg8(fd, 0x00, (old_value | 0x10));
}

int PCA9685_output::unset_sleep()
{
	int old_value = wiringPiI2CReadReg8(fd, 0x00);
	return wiringPiI2CWriteReg8(fd, 0x00, (old_value & 0xEF));
}

void PCA9685_output::set_PWM_ON(const int ch, const unsigned short int value)
{
	int low_byte = value & 0x00FF;
	int high_byte = (value & 0x0F00) >> 8;
	int reg = 0x06 + 4 * ch;
	wiringPiI2CWriteReg8(fd, reg, low_byte);
	wiringPiI2CWriteReg8(fd, reg + 1, high_byte);
}

void PCA9685_output::set_PWM_OFF(const int ch, unsigned short int value)
{
	//offset
	value += 45;
	int low_byte = value & 0x00FF;
	int high_byte = (value & 0x0F00) >> 8;
	int reg = 0x08 + 4 * ch;
	wiringPiI2CWriteReg8(fd, reg, low_byte);
	wiringPiI2CWriteReg8(fd, reg + 1, high_byte);
/*	std::cout << wiringPiI2CReadReg8(fd, reg) << std::endl;
	std::cout << wiringPiI2CReadReg8(fd, reg + 1) << std::endl;
	std::cout << wiringPiI2CReadReg8(fd, 0x00) << std::endl;
	std::cout << wiringPiI2CReadReg8(fd, 0xFE) << std::endl;*/
}
