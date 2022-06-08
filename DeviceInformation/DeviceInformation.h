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
		static const std::vector<std::string> getHDDSerial();
		static const std::string getProcessorInformation();
		static const ULONGLONG getRAMSize();
		static const ULONGLONG getHDDTotalSize(std::string drive);
		static const ULONGLONG getHDDAvailableSize(std::string drive);
		static const ULONGLONG getHDDUsingSize(std::string drive);
		static const double getHDDUsage(std::string drive);
		static const std::string getMACAddress();
	protected:
	private:
		static inline std::string& ltrim(std::string& s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
		}

		//뒤에 있는 개행 문자 제거
		static inline std::string& rtrim(std::string& s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
		}

		//양쪽 끝의 개행 문자 제거
		static inline std::string& trim(std::string& s) {
			return ltrim(rtrim(s));
		}

		static const BOOL GetDiskFreeSpaceInformation(std::string drive, PULARGE_INTEGER freeBytesAvailableToCaller, PULARGE_INTEGER totalNumberOfBytes, PULARGE_INTEGER totalNumberOfFreeBytes);
	};
}
#endif	// _DEVICEINFORMATION_H_