#include <wiringSerial.h>
#include <fstream>
#include <string>
#include <vector>

class Serial_reader
{
private:
	int fd;
	std::ifstream device;
	int size;
	std::string input;
public:
	Serial_reader(const char *dev_name, int baud, int sz);
	std::vector<float> read_float_data();
};
