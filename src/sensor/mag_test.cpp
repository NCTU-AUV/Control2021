#include "serial_reader/Serial_reader.h"
#include <iostream>
#include <vector>
#include <fstream>

const int data_n = 6;


int main(int argc, char **argv)
{
	bool start;
	std::ofstream outfile;
	std::vector<float> data;
	Serial_reader imu_reader("/dev/ttyACM0", 115200, data_n);

	outfile.open("mag_test_data.csv");
	std::cout << "begin reading data\n";
	std::cin >> start;
	while (start)
	{
		data = imu_reader.read_float_data();
		for (int i = 0; i < data_n; i++)
		{
			std::cout << data[i] << " ";
			outfile << data[i] << ",";
		}
		std::cout << std::endl;
		outfile << std::endl;
	}
	return 0;
}

