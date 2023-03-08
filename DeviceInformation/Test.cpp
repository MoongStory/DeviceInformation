#include "DeviceInformation.h"

int main()
{
	try
	{
		std::string computer_name;
		MOONG::DeviceInformation::get_computer_name(computer_name, ComputerNameDnsFullyQualified);
		std::cout << "GetComputerName[" << computer_name.c_str() << "]" << std::endl;

		std::cout << std::endl;

		std::vector<std::string> hdd_serial_number_list = MOONG::DeviceInformation::get_hdd_serial();
		for (std::vector<std::string>::iterator iter_hdd_serial_number_list = hdd_serial_number_list.begin(); iter_hdd_serial_number_list != hdd_serial_number_list.end(); ++iter_hdd_serial_number_list)
		{
			std::cout << "getHDDSerial[" << (*iter_hdd_serial_number_list).c_str() << "]" << std::endl;
		}

		std::cout << std::endl;

		std::cout << "getProcessorInformation[" << MOONG::DeviceInformation::get_processor_information().c_str() << "]" << std::endl;

		std::cout << std::endl;

		ULONGLONG ram_size = MOONG::DeviceInformation::get_ram_size();
		std::cout << "getRAMSize[" << ram_size << " KB]" << std::endl;
		std::cout << "getRAMSize[" << ram_size / 1024 << " MB]" << std::endl;
		std::cout << "getRAMSize[" << ram_size / 1024 / 1024 << " GB]" << std::endl;

		std::cout << std::endl;

		char driveChar[256] = { 0 };
		int count = GetLogicalDriveStringsA(256, driveChar);

		for (int i = 0; i < count / 4; i++)
		{
			std::cout << "HDD 전체 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::get_hdd_total_size(driveChar + i * 4) << " byte], Usage[" << MOONG::DeviceInformation::get_hdd_usage(driveChar + i * 4) << "]" << std::endl;
			std::cout << "HDD 전체 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::get_hdd_total_size(driveChar + i * 4) / 1024 << " KB]" << std::endl;
			std::cout << "HDD 전체 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::get_hdd_total_size(driveChar + i * 4) / 1024 / 1024 << " MB]" << std::endl;
			std::cout << "HDD 전체 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::get_hdd_total_size(driveChar + i * 4) / 1024 / 1024 / 1024 << " GB]\n" << std::endl;

			std::cout << "HDD 사용 가능한 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::get_hdd_available_size(driveChar + i * 4) << " byte]" << std::endl;
			std::cout << "HDD 사용중인 사이즈 " << driveChar + i * 4 << "[" << MOONG::DeviceInformation::get_hdd_using_size(driveChar + i * 4) << " byte]" << std::endl;

			std::cout << std::endl;
			std::cout << std::endl;
		}

		std::cout << "HDD 전체 사이즈 " << "c:\\[" << MOONG::DeviceInformation::get_hdd_total_size("c") << " byte], Usage[" << MOONG::DeviceInformation::get_hdd_usage("c") << "]" << std::endl;
		std::cout << "HDD 전체 사이즈 " << "C:\\[" << MOONG::DeviceInformation::get_hdd_total_size("C") << " byte], Usage[" << MOONG::DeviceInformation::get_hdd_usage("C") << "]" << std::endl;
		std::cout << "HDD 전체 사이즈 " << "Z:\\[" << MOONG::DeviceInformation::get_hdd_total_size("Z") << " byte], Usage[" << MOONG::DeviceInformation::get_hdd_usage("Z") << "]" << std::endl;

		std::cout << std::endl;

		std::cout << "HDD 사용 가능한 사이즈 " << "C:\\[" << MOONG::DeviceInformation::get_hdd_available_size("C") << " byte]" << std::endl;
		std::cout << "HDD 사용 가능한 사이즈 " << "Z:\\[" << MOONG::DeviceInformation::get_hdd_available_size("Z") << " byte]\n" << std::endl;

		std::cout << "HDD 사용중인 사이즈 " << "C:\\[" << MOONG::DeviceInformation::get_hdd_using_size("C") << " byte]" << std::endl;
		std::cout << "HDD 사용중인 사이즈 " << "Z:\\[" << MOONG::DeviceInformation::get_hdd_using_size("Z") << " byte]" << std::endl;

		std::cout << std::endl;

		std::string mac_address = MOONG::DeviceInformation::get_mac_address();
		std::cout << "getMACAddress[" << mac_address.c_str() << "]" << std::endl;

		std::cout << std::endl;

		std::vector<std::string> mac_address_list = MOONG::DeviceInformation::get_mac_address_all();
		std::cout << "getMACAddressAll" << std::endl;
		for (std::vector<std::string>::iterator iter_mac_address_list = mac_address_list.begin(); iter_mac_address_list != mac_address_list.end(); ++iter_mac_address_list)
		{
			std::cout << "getMACAddress[" << (*iter_mac_address_list).c_str() << "]" << std::endl;
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}

	return EXIT_SUCCESS;
}