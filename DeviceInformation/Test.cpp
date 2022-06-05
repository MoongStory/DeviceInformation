#include "DeviceInformation.h"

int main()
{
	std::cout << MOONG::DeviceInformation::getHDDSerial() << std::endl;

	std::cout << MOONG::DeviceInformation::getProcessorInformation() << std::endl;

	std::cout << MOONG::DeviceInformation::getRAMSize() << std::endl;

	std::cout << MOONG::DeviceInformation::getHDDSize() << std::endl;
}