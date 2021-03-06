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
		static const std::vector<std::string> getMACAddressAll();
	protected:
	private:
		// trim from start (in place)
		static inline void ltrim(std::string& s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
				return !std::isspace(ch);
				}));
		}

		// trim from end (in place)
		static inline void rtrim(std::string& s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
				}).base(), s.end());
		}

		// trim from both ends (in place)
		static inline void trim(std::string& s) {
			ltrim(s);
			rtrim(s);
		}

		// trim from start (copying)
		static inline std::string ltrim_copy(std::string s) {
			ltrim(s);
			return s;
		}

		// trim from end (copying)
		static inline std::string rtrim_copy(std::string s) {
			rtrim(s);
			return s;
		}

		// trim from both ends (copying)
		static inline std::string trim_copy(std::string s) {
			trim(s);
			return s;
		}

		static const BOOL GetDiskFreeSpaceInformation(std::string drive, PULARGE_INTEGER freeBytesAvailableToCaller, PULARGE_INTEGER totalNumberOfBytes, PULARGE_INTEGER totalNumberOfFreeBytes);
	};
}
#endif	// _DEVICEINFORMATION_H_