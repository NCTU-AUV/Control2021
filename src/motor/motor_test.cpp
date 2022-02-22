#include "pca9685_output/pca9685_output.h"
#include <iostream>

void reset(PCA9685_output &p);

int main()
{
        PCA9685_output motor(233.5);

	int ch = -1;
        unsigned short int signal = 1500;
        while (std::cin >> ch >> signal)
        {
		if (ch == -1)
                	for (int i = 0; i < 6; i++)
                		motor.set_PWM_OFF(i, signal);
		else
		{
			reset(motor);
			motor.set_PWM_OFF(ch, signal);
		}
        }

	reset(motor);
        return 0;
}


void reset(PCA9685_output &p)
{
        for (int i = 0; i < 6; i++)
        	p.set_PWM_OFF(i, 1500);
}
