#include "DeviceInformation.h"

int main()
{
	std::cout << "getHDDSerial[" << MOONG::DeviceInformation::getHDDSerial() << "]\n" << std::endl;

	std::cout << "getProcessorInformation[" << MOONG::DeviceInformation::getProcessorInformation() << "]\n" << std::endl;

	std::cout << "getRAMSize[" << MOONG::DeviceInformation::getRAMSize() << " KB]" << std::endl;
	std::cout << "getRAMSize[" << MOONG::DeviceInformation::getRAMSize() / 1024 << " MB]" << std::endl;
	std::cout << "getRAMSize[" << MOONG::DeviceInformation::getRAMSize() / 1024 / 1024 << " GB]\n" << std::endl;

	char driveChar[256] = { 0 };
	int count = GetLogicalDriveStringsA(256, driveChar);
	for (int i = 0; i < count / 4; i++)
	{
		std::cout << "HDD 전체 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDTotalSize(driveChar + i * 4) << " byte], Usage[" << MOONG::DeviceInformation::getHDDUsage(driveChar + i * 4) << "]" << std::endl;
		std::cout << "HDD 전체 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDTotalSize(driveChar + i * 4) / 1024 << " KB]" << std::endl;
		std::cout << "HDD 전체 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDTotalSize(driveChar + i * 4) / 1024 / 1024 << " MB]" << std::endl;
		std::cout << "HDD 전체 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDTotalSize(driveChar + i * 4) / 1024 / 1024 / 1024 << " GB]\n" << std::endl;

		std::cout << "HDD 사용 가능한 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDAvailableSize(driveChar + i * 4) << " byte]" << std::endl;
		std::cout << "HDD 사용중인 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDUsingSize(driveChar + i * 4) << " byte]\n\n" << std::endl;
	}

	std::cout << "HDD 전체 사이즈 " << "c:\\[" << MOONG::DeviceInformation::getHDDTotalSize("c") << " byte], Usage[" << MOONG::DeviceInformation::getHDDUsage("c") << "]" << std::endl;
	std::cout << "HDD 전체 사이즈 " << "C:\\[" << MOONG::DeviceInformation::getHDDTotalSize("C") << " byte], Usage[" << MOONG::DeviceInformation::getHDDUsage("C") << "]" << std::endl;
	std::cout << "HDD 전체 사이즈 " << "Z:\\[" << MOONG::DeviceInformation::getHDDTotalSize("Z") << " byte], Usage[" << MOONG::DeviceInformation::getHDDUsage("Z") << "]\n" << std::endl;

	std::cout << "HDD 사용 가능한 사이즈 " << "C:\\[" << MOONG::DeviceInformation::getHDDAvailableSize("C") << " byte]" << std::endl;
	std::cout << "HDD 사용 가능한 사이즈 " << "Z:\\[" << MOONG::DeviceInformation::getHDDAvailableSize("Z") << " byte]\n" << std::endl;

	std::cout << "HDD 사용중인 사이즈 " << "C:\\[" << MOONG::DeviceInformation::getHDDUsingSize("C") << " byte]" << std::endl;
	std::cout << "HDD 사용중인 사이즈 " << "Z:\\[" << MOONG::DeviceInformation::getHDDUsingSize("Z") << " byte]" << std::endl;
}