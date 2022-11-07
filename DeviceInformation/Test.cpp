#include "DeviceInformation.h"

int main()
{
	std::vector<std::string> hdd_serial_number_list = MOONG::DeviceInformation::getHDDSerial();
	for (std::vector<std::string>::iterator iter_hdd_serial_number_list = hdd_serial_number_list.begin(); iter_hdd_serial_number_list != hdd_serial_number_list.end(); ++iter_hdd_serial_number_list)
	{
		std::cout << "getHDDSerial[" << *iter_hdd_serial_number_list << "]" << std::endl;
	}

	std::cout << std::endl;

	try
	{
		std::cout << "getProcessorInformation[" << MOONG::DeviceInformation::getProcessorInformation() << "]\n" << std::endl;
	}
	catch (const MOONG::Exception& exception)
	{
		std::cout << exception.getReason().c_str() << std::endl;
	}

	try
	{
		ULONGLONG ram_size = MOONG::DeviceInformation::getRAMSize();
		std::cout << "getRAMSize[" << ram_size << " KB]" << std::endl;
		std::cout << "getRAMSize[" << ram_size / 1024 << " MB]" << std::endl;
		std::cout << "getRAMSize[" << ram_size / 1024 / 1024 << " GB]" << std::endl;
	}
	catch (const MOONG::Exception& exception)
	{
		std::cout << exception.getReason().c_str() << std::endl;
	}

	std::cout << std::endl;

	char driveChar[256] = { 0 };
	int count = GetLogicalDriveStringsA(256, driveChar);

	try
	{
		for (int i = 0; i < count / 4; i++)
		{
			std::cout << "HDD ��ü ������ " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDTotalSize(driveChar + i * 4) << " byte], Usage[" << MOONG::DeviceInformation::getHDDUsage(driveChar + i * 4) << "]" << std::endl;
			std::cout << "HDD ��ü ������ " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDTotalSize(driveChar + i * 4) / 1024 << " KB]" << std::endl;
			std::cout << "HDD ��ü ������ " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDTotalSize(driveChar + i * 4) / 1024 / 1024 << " MB]" << std::endl;
			std::cout << "HDD ��ü ������ " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDTotalSize(driveChar + i * 4) / 1024 / 1024 / 1024 << " GB]\n" << std::endl;

			std::cout << "HDD ��� ������ ������ " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDAvailableSize(driveChar + i * 4) << " byte]" << std::endl;
			std::cout << "HDD ������� ������ " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::getHDDUsingSize(driveChar + i * 4) << " byte]\n\n" << std::endl;
		}

		std::cout << "HDD ��ü ������ " << "c:\\[" << MOONG::DeviceInformation::getHDDTotalSize("c") << " byte], Usage[" << MOONG::DeviceInformation::getHDDUsage("c") << "]" << std::endl;
		std::cout << "HDD ��ü ������ " << "C:\\[" << MOONG::DeviceInformation::getHDDTotalSize("C") << " byte], Usage[" << MOONG::DeviceInformation::getHDDUsage("C") << "]" << std::endl;
		std::cout << "HDD ��ü ������ " << "Z:\\[" << MOONG::DeviceInformation::getHDDTotalSize("Z") << " byte], Usage[" << MOONG::DeviceInformation::getHDDUsage("Z") << "]\n" << std::endl;

		std::cout << "HDD ��� ������ ������ " << "C:\\[" << MOONG::DeviceInformation::getHDDAvailableSize("C") << " byte]" << std::endl;
		std::cout << "HDD ��� ������ ������ " << "Z:\\[" << MOONG::DeviceInformation::getHDDAvailableSize("Z") << " byte]\n" << std::endl;

		std::cout << "HDD ������� ������ " << "C:\\[" << MOONG::DeviceInformation::getHDDUsingSize("C") << " byte]" << std::endl;
		std::cout << "HDD ������� ������ " << "Z:\\[" << MOONG::DeviceInformation::getHDDUsingSize("Z") << " byte]\n" << std::endl;
	}
	catch (const MOONG::Exception& exception)
	{
		std::cout << exception.getReason().c_str() << std::endl;
	}

	try
	{
		std::string mac_address = MOONG::DeviceInformation::getMACAddress();
		std::cout << "getMACAddress[" << mac_address.c_str() << "]" << std::endl;
	}
	catch (MOONG::Exception& exception)
	{
		std::cout << exception.getReason().c_str() << std::endl;
	}

	std::cout << std::endl;

	try
	{
		std::vector<std::string> mac_address_list = MOONG::DeviceInformation::getMACAddressAll();
		std::cout << "getMACAddressAll" << std::endl;
		for (std::vector<std::string>::iterator iter_mac_address_list = mac_address_list.begin(); iter_mac_address_list != mac_address_list.end(); ++iter_mac_address_list)
		{
			std::cout << "getMACAddress[" << *iter_mac_address_list << "]" << std::endl;
		}
	}
	catch (const MOONG::Exception& exception)
	{
		std::cout << exception.getReason().c_str() << std::endl;
	}

	std::cout << std::endl;

	return EXIT_SUCCESS;
}