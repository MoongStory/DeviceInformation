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
		static const bool GetComputerName(std::string& computer_name, const COMPUTER_NAME_FORMAT NameType = ComputerNamePhysicalDnsHostname);
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