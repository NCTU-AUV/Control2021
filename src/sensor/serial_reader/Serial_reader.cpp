#include "Serial_reader.h"

Serial_reader::Serial_reader(const char *dev_name, int buad, int sz)
{
	fd = serialOpen(dev_name, buad);
	device.open(dev_name);
	size = sz;
}

std::vector<float> Serial_reader::read_float_data()
{
	float n = 0;
	std::vector<float> output;
	
	input = "";
	while (input.length() != (size * 4))
		getline(device, input);
	for (int i = 0; i < (size * 4); i++)
	{
		((char*)& n)[i % 4] = input[i];
		if ((i % 4) == 3)
			output.push_back(n);
	}
	return output;
}
