/*******************************************************************************
MIT License

Copyright (c) 2023 Moong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

// https://github.com/MoongStory/DeviceInformation

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _DEVICEINFORMATION_H_
#define _DEVICEINFORMATION_H_

#include <Windows.h>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

namespace MOONG
{
	class DeviceInformation
	{
	public:
	protected:
	private:

	public:
		static const bool get_computer_name(std::string& computer_name, const COMPUTER_NAME_FORMAT NameType = ComputerNamePhysicalDnsHostname);
		static const ULONGLONG get_hdd_available_size(std::string drive);
		static const std::vector<std::string> get_hdd_serial();
		static const ULONGLONG get_hdd_total_size(std::string drive);
		static const double get_hdd_usage(std::string drive);
		static const ULONGLONG get_hdd_using_size(std::string drive);
		static const std::string get_mac_address();
		static const std::vector<std::string> get_mac_address_all();
		static const std::string get_processor_information();
		static const ULONGLONG get_ram_size();
	protected:
	private:
		static const BOOL get_disk_free_space_information(std::string drive, PULARGE_INTEGER freeBytesAvailableToCaller, PULARGE_INTEGER totalNumberOfBytes, PULARGE_INTEGER totalNumberOfFreeBytes);
	};
}
#endif	// _DEVICEINFORMATION_H_