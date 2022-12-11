#pragma once
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
		static const bool GetComputerName(std::string& computer_name);
		static const ULONGLONG GetHDDAvailableSize(std::string drive);
		static const std::vector<std::string> GetHDDSerial();
		static const ULONGLONG GetHDDTotalSize(std::string drive);
		static const double GetHDDUsage(std::string drive);
		static const ULONGLONG GetHDDUsingSize(std::string drive);
		static const std::string GetMACAddress();
		static const std::vector<std::string> GetMACAddressAll();
		static const std::string GetProcessorInformation();
		static const ULONGLONG GetRAMSize();
	protected:
	private:
		static const BOOL GetDiskFreeSpaceInformation(std::string drive, PULARGE_INTEGER freeBytesAvailableToCaller, PULARGE_INTEGER totalNumberOfBytes, PULARGE_INTEGER totalNumberOfFreeBytes);
	};
}
#endif	// _DEVICEINFORMATION_H_