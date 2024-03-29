#include "DeviceInformation.h"

#include "../../Registry/Registry/Registry.h"
#include "../../ConvertDataType/ConvertDataType/ConvertDataType.h"
#include "../../StringTool/StringTool/StringTool.h"

#include <string>
#include <Iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#include <WinSock.h>
#pragma comment(lib, "Ws2_32.lib")

#include <strsafe.h>

const bool MOONG::DeviceInformation::get_computer_name(std::string& computer_name, const COMPUTER_NAME_FORMAT NameType/* = ComputerNamePhysicalDnsHostname*/)
{
	char buffer[256] = {0};
	DWORD size = sizeof(buffer);

	bool return_value = GetComputerNameExA(NameType, buffer, &size) ? true : false;

	computer_name = buffer;

	return return_value;
}

const ULONGLONG MOONG::DeviceInformation::get_hdd_available_size(std::string drive)
{
	ULARGE_INTEGER freeBytesAvailableToCaller = { 0 };
	ULARGE_INTEGER totalNumberOfBytes = { 0 };
	ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };

	MOONG::DeviceInformation::get_disk_free_space_information(drive, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);

	return totalNumberOfFreeBytes.QuadPart;
}

const std::vector<std::string> MOONG::DeviceInformation::get_hdd_serial()
{
	// CMD창에서 wmic path win32_physicalmedia get serialnumber

	// Reg에서 값을 얻어오는 방법 (되도록 레지에서 해결)
	const std::string szSubKey = "HARDWARE\\DEVICEMAP\\Scsi";
	std::vector<std::string> VectorScsiPort;
	std::string tempSubKey;
	std::string hdd_serial_number;
	std::vector<std::string> vector_hdd_serial_number;

	if (MOONG::Registry::getRegSubKeys(HKEY_LOCAL_MACHINE, szSubKey, VectorScsiPort) == EXIT_SUCCESS)
	{
		for (std::vector<std::string>::iterator iterScsiPort = VectorScsiPort.begin(); iterScsiPort != VectorScsiPort.end(); ++iterScsiPort)
		{
			tempSubKey = *iterScsiPort;
			std::transform(tempSubKey.begin(), tempSubKey.end(), tempSubKey.begin(), tolower);

			if ((tempSubKey).find("scsi port") != std::string::npos)
			{
				std::string strSubKeyScsiPort = szSubKey;
				strSubKeyScsiPort += "\\";
				strSubKeyScsiPort += *iterScsiPort;

				std::vector<std::string> VectorScsiBus;

				if (MOONG::Registry::getRegSubKeys(HKEY_LOCAL_MACHINE, strSubKeyScsiPort, VectorScsiBus) == EXIT_SUCCESS)
				{
					for (std::vector<std::string>::iterator iterScsiBus = VectorScsiBus.begin(); iterScsiBus != VectorScsiBus.end(); ++iterScsiBus)
					{
						//std::cout << *iterScsiBus << std::endl;
						tempSubKey = *iterScsiBus;
						std::transform(tempSubKey.begin(), tempSubKey.end(), tempSubKey.begin(), tolower);

						if ((tempSubKey).find("scsi bus") != std::string::npos)
						{
							std::string strSubKeyScsiBus = strSubKeyScsiPort;
							strSubKeyScsiBus += "\\";
							strSubKeyScsiBus += *iterScsiBus;

							//std::cout << strSubKeyScsiBus << std::endl;

							std::vector<std::string> VectorTargetId;

							if (MOONG::Registry::getRegSubKeys(HKEY_LOCAL_MACHINE, strSubKeyScsiBus, VectorTargetId) == EXIT_SUCCESS)
							{
								for (std::vector<std::string>::iterator iterTargetId = VectorTargetId.begin(); iterTargetId != VectorTargetId.end(); ++iterTargetId)
								{
									//std::cout << *iterTargetId << std::endl;
									tempSubKey = *iterTargetId;
									std::transform(tempSubKey.begin(), tempSubKey.end(), tempSubKey.begin(), tolower);

									if ((tempSubKey).find("target id") != std::string::npos)
									{
										std::string strSubKeyTargetId = strSubKeyScsiBus;
										strSubKeyTargetId += "\\";
										strSubKeyTargetId += *iterTargetId;

										//std::cout << strSubKeyTargetId << std::endl;

										std::string strSubKey;
										strSubKey = strSubKeyTargetId;
										strSubKey += "\\Logical Unit Id 0";

										if(MOONG::Registry::Read(HKEY_LOCAL_MACHINE, strSubKey, "SerialNumber", hdd_serial_number) != ERROR_SUCCESS)
										{
											//std::string error_message = "Can't open subkey [";
											//error_message += strSubKey;
											//error_message += "]";

											//std::cout << error_message.c_str() << std::endl;

											//error_message.clear();

											continue;
										}
										else
										{
											if (hdd_serial_number.length() > 0)
											{
												MOONG::StringTool::trim(hdd_serial_number);

												vector_hdd_serial_number.push_back(hdd_serial_number);
											}
										}
									}
								}
							}
							else
							{
								std::cout << strSubKeyScsiBus << " 하위 키가 없습니다." << std::endl;
							}
						}
					}
				}
				else
				{
					std::cout << strSubKeyScsiPort << " 하위 키가 없습니다." << std::endl;
				}
			}
		}
	}
	else
	{
		std::cout << szSubKey << " 하위 키가 없습니다." << std::endl;
	}

	return vector_hdd_serial_number;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HRESULT hres;
	//std::string returnVal;
	//const std::string errorMsg("Failed to get HDD serial");

	//// Step 1: --------------------------------------------------
	//// Initialize COM. ------------------------------------------

	//hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
	//if (FAILED(hres))
	//{
	//	std::cout << "Failed to initialize COM library. Error code = 0x" << std::hex << hres << std::endl;

	//	return "Failed to get HDD serial";	// Program has failed.
	//}

	//// Step 2: --------------------------------------------------
	//// Set general COM security levels --------------------------

	//hres =  CoInitializeSecurity(
	//	NULL, 
	//	-1,                          // COM authentication
	//	NULL,                        // Authentication services
	//	NULL,                        // Reserved
	//	RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
	//	RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
	//	NULL,                        // Authentication info
	//	EOAC_NONE,                   // Additional capabilities 
	//	NULL                         // Reserved
	//	);


	//if (FAILED(hres))
	//{
	//	std::cout << "Failed to initialize security. Error code = 0x" << std::hex << hres << std::endl;

	//	CoUninitialize();

	//	return errorMsg;	// Program has failed.
	//}

	//// Step 3: ---------------------------------------------------
	//// Obtain the initial locator to WMI -------------------------

	//IWbemLocator *pLoc = NULL;

	//hres = CoCreateInstance(
	//	CLSID_WbemLocator,             
	//	0, 
	//	CLSCTX_INPROC_SERVER, 
	//	IID_IWbemLocator, (LPVOID *) &pLoc);

	//if (FAILED(hres))
	//{
	//	std::cout << "Failed to create IWbemLocator object." << " Err code = 0x" << std::hex << hres << std::endl;

	//	CoUninitialize();

	//	return errorMsg;	// Program has failed.
	//}

	//// Step 4: -----------------------------------------------------
	//// Connect to WMI through the IWbemLocator::ConnectServer method

	//IWbemServices *pSvc = NULL;

	//// Connect to the root\cimv2 namespace with
	//// the current user and obtain pointer pSvc
	//// to make IWbemServices calls.
	//hres = pLoc->ConnectServer(
	//	_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
	//	NULL,                    // User name. NULL = current user
	//	NULL,                    // User password. NULL = current
	//	0,                       // Locale. NULL indicates current
	//	NULL,                    // Security flags.
	//	0,                       // Authority (for example, Kerberos)
	//	0,                       // Context object 
	//	&pSvc                    // pointer to IWbemServices proxy
	//	);

	//if (FAILED(hres))
	//{
	//	std::cout << "Could not connect. Error code = 0x" << std::hex << hres << std::endl;

	//	pLoc->Release();

	//	CoUninitialize();

	//	return errorMsg;	// Program has failed.
	//}

	//std::cout << "Connected to ROOT\\CIMV2 WMI namespace" << std::endl;


	//// Step 5: --------------------------------------------------
	//// Set security levels on the proxy -------------------------

	//hres = CoSetProxyBlanket(
	//	pSvc,                        // Indicates the proxy to set
	//	RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
	//	RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
	//	NULL,                        // Server principal name 
	//	RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
	//	RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
	//	NULL,                        // client identity
	//	EOAC_NONE                    // proxy capabilities 
	//	);

	//if (FAILED(hres))
	//{
	//	std::cout << "Could not set proxy blanket. Error code = 0x" << std::hex << hres << std::endl;

	//	pSvc->Release();
	//	pLoc->Release();

	//	CoUninitialize();

	//	return errorMsg;	// Program has failed.
	//}

	//// Step 6: --------------------------------------------------
	//// Use the IWbemServices pointer to make requests of WMI ----

	//// For example, get the name of the operating system
	//IEnumWbemClassObject* pEnumerator = NULL;
	//hres = pSvc->ExecQuery(
	//	bstr_t("WQL"), 
	//	bstr_t("SELECT SerialNumber FROM Win32_PhysicalMedia"),
	//	WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
	//	NULL,
	//	&pEnumerator);

	//if (FAILED(hres))
	//{
	//	std::cout << "Query for operating system name failed." << " Error code = 0x" << std::hex << hres << std::endl;

	//	pSvc->Release();
	//	pLoc->Release();

	//	CoUninitialize();

	//	return errorMsg;	// Program has failed.
	//}

	//// Step 7: -------------------------------------------------
	//// Get the data from the query in step 6 -------------------

	//IWbemClassObject *pclsObj = NULL;
	//ULONG uReturn = 0;

	////std::cout << "### 테스트 HDD Serial" << std::endl;

	//while (pEnumerator)
	//{
	//	HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
	//		&pclsObj, &uReturn);

	//	if(0 == uReturn)
	//	{
	//		break;
	//	}

	//	VARIANT vtProp;

	//	// Get the value of the Name property
	//	hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
	//	//std::wcout << " HDD Serial : " << vtProp.bstrVal << std::endl;

	//	std::string strTemp = (const char*)_bstr_t(V_BSTR(&vtProp));
	//	returnVal += trim(strTemp);
	//	returnVal += ";";

	//	VariantClear(&vtProp);

	//	pclsObj->Release();
	//}
	////std::cout << "### 테스트 HDD Serial end\n" << std::endl;

	//// Cleanup
	//pSvc->Release();
	//pLoc->Release();
	//pEnumerator->Release();
	//CoUninitialize();

	//return returnVal;   // Program successfully completed.
}

const ULONGLONG MOONG::DeviceInformation::get_hdd_total_size(std::string drive)
{
	ULARGE_INTEGER freeBytesAvailableToCaller = { 0 };
	ULARGE_INTEGER totalNumberOfBytes = { 0 };
	ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };

	MOONG::DeviceInformation::get_disk_free_space_information(drive, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);

	return totalNumberOfBytes.QuadPart;
}

const double MOONG::DeviceInformation::get_hdd_usage(std::string drive)
{
	ULARGE_INTEGER freeBytesAvailableToCaller = { 0 };
	ULARGE_INTEGER totalNumberOfBytes = { 0 };
	ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };

	double hdd_usage = 0;

	if (MOONG::DeviceInformation::get_disk_free_space_information(drive, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes) == TRUE)
	{
#if _MSC_VER > 1200
		hdd_usage = ((double)totalNumberOfBytes.QuadPart - (double)totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100.0;
#else
		hdd_usage = (MOONG::ConvertDataType::unsigned_int64_to_double(totalNumberOfBytes.QuadPart) - MOONG::ConvertDataType::unsigned_int64_to_double(totalNumberOfFreeBytes.QuadPart)) / MOONG::ConvertDataType::unsigned_int64_to_double(totalNumberOfBytes.QuadPart) * 100.0;
#endif
	}

	return hdd_usage;
}

const ULONGLONG MOONG::DeviceInformation::get_hdd_using_size(std::string drive)
{
	ULARGE_INTEGER freeBytesAvailableToCaller = { 0 };
	ULARGE_INTEGER totalNumberOfBytes = { 0 };
	ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };

	MOONG::DeviceInformation::get_disk_free_space_information(drive, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);

	return totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart;
}

const std::string MOONG::DeviceInformation::get_mac_address()
{
	// Declare and initialize variables.
	std::string mac_address;
	char mac_address_hexadecimal_format[18] = { 0 };
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0, j = 0;

	// variables used for GetIfTable and GetIfEntry.
	MIB_IFTABLE* pIfTable = NULL;
	MIB_IFROW* pIfRow = NULL;

	// Allocate memory for our pointers.
	pIfTable = (MIB_IFTABLE*)HeapAlloc(GetProcessHeap(), 0, (sizeof(MIB_IFTABLE)));
	if (pIfTable == NULL)
	{
		return "Error allocating memory needed to call GetIfTable";
	}

	// Make an initial call to GetIfTable to get the necessary size into dwSize.
	dwSize = sizeof(MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
	{
		HeapFree(GetProcessHeap(), 0, (pIfTable));

		pIfTable = (MIB_IFTABLE*)HeapAlloc(GetProcessHeap(), 0, (dwSize));

		if (pIfTable == NULL)
		{
			return "Error allocating memory needed to call GetIfTable";
		}
	}

	// Make a second call to GetIfTable to get the actual data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR)
	{
		//printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);

		for (i = 0; i < pIfTable->dwNumEntries; i++)
		{
			pIfRow = (MIB_IFROW*)&pIfTable->table[i];

			// "WAN" 필터링.
			if (strstr((const char*)pIfRow->bDescr, "Loop") || strstr((const char*)pIfRow->bDescr, "PPP") || strstr((const char*)pIfRow->bDescr, "WAN"))
			{
				continue;
			}

			// 무선 LAN 필터링.
			if ((pIfRow->dwPhysAddrLen == 6) && (pIfRow->dwType == MIB_IF_TYPE_ETHERNET || pIfRow->dwType == IF_TYPE_IEEE80211))
			{
				if (pIfRow->dwOperStatus != IF_OPER_STATUS_OPERATIONAL)
				{
					continue;
				}

				ZeroMemory(mac_address_hexadecimal_format, sizeof(mac_address_hexadecimal_format));

				StringCbPrintfA(mac_address_hexadecimal_format, sizeof(mac_address_hexadecimal_format), "%02X-%02X-%02X-%02X-%02X-%02X",
					pIfRow->bPhysAddr[0],
					pIfRow->bPhysAddr[1],
					pIfRow->bPhysAddr[2],
					pIfRow->bPhysAddr[3],
					pIfRow->bPhysAddr[4],
					pIfRow->bPhysAddr[5]);

				mac_address += mac_address_hexadecimal_format;

				if (mac_address.empty() != true)
				{
					break;
				}
			}
		}
	}
	else
	{
		if (pIfTable != NULL)
		{
			HeapFree(GetProcessHeap(), 0, (pIfTable));

			pIfTable = NULL;
		}

		mac_address = "GetIfTable failed with error[";
		mac_address += MOONG::ConvertDataType::to_string(dwRetVal);
		mac_address += "]";

		return mac_address;
	}

	if (pIfTable != NULL)
	{
		HeapFree(GetProcessHeap(), 0, (pIfTable));

		pIfTable = NULL;
	}

	return mac_address;
}

const std::vector<std::string> MOONG::DeviceInformation::get_mac_address_all()
{
	// It is possible for an adapter to have multiple
	// IPv4 addresses, gateways, and secondary WINS servers
	// assigned to the adapter. 
	//
	// Note that this sample code only prints out the 
	// first entry for the IP address/mask, and gateway, and
	// the primary and secondary WINS server for each adapter. 

	// Declare and initialize variables.
	std::vector<std::string> mac_address_list;
	char mac_address_hexadecimal_format[18] = { 0 };
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i = 0;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	pAdapterInfo = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), 0, (sizeof(IP_ADAPTER_INFO)));

	if (pAdapterInfo == NULL)
	{
		mac_address_list.push_back("Error allocating memory needed to call GetAdaptersinfo #0");

		return mac_address_list;
	}

	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable.
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		HeapFree(GetProcessHeap(), 0, (pAdapterInfo));

		pAdapterInfo = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), 0, (ulOutBufLen));

		if (pAdapterInfo == NULL)
		{
			mac_address_list.push_back("Error allocating memory needed to call GetAdaptersinfo #1");

			return mac_address_list;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;

		while (pAdapter)
		{
			ZeroMemory(mac_address_hexadecimal_format, sizeof(mac_address_hexadecimal_format));

			StringCchPrintfA(mac_address_hexadecimal_format, sizeof(mac_address_hexadecimal_format), "%02X-%02X-%02X-%02X-%02X-%02X",
				pAdapter->Address[0], pAdapter->Address[1],
				pAdapter->Address[2], pAdapter->Address[3],
				pAdapter->Address[4], pAdapter->Address[5]);

			if (strcmp(mac_address_hexadecimal_format, "00-00-00-00-00-00") != 0)
			{
				mac_address_list.push_back(mac_address_hexadecimal_format);
			}

			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		std::string error_message;
		error_message = "GetAdaptersInfo failed with error[";
		error_message += MOONG::ConvertDataType::to_string(dwRetVal);
		error_message += "]";

		mac_address_list.push_back(error_message);

		return mac_address_list;
	}

	if (pAdapterInfo)
	{
		HeapFree(GetProcessHeap(), 0, (pAdapterInfo));
	}

	return mac_address_list;
}

const std::string MOONG::DeviceInformation::get_processor_information()
{
	std::string processor_information;

	const std::string sub_key = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
	const std::string value = "ProcessorNameString";

	MOONG::Registry::Read(HKEY_LOCAL_MACHINE, sub_key, value, processor_information);

	return processor_information;
}

const ULONGLONG MOONG::DeviceInformation::get_ram_size()
{
	ULONGLONG ram_size = 0;

#if _MSC_VER > 1200
	GetPhysicallyInstalledSystemMemory(&ram_size);

	//std::cout << "\n### 테스트 RAM Size" << std::endl;
	//std::cout << ramSize << " KB" << std::endl;
	//std::cout << ramSize / 1024 << " MB" << std::endl;
	//std::cout << ramSize / 1024 / 1024 << " GB" << std::endl;
	//std::cout << "### 테스트 RAM Size end\n" << std::endl;
#endif

	return ram_size;
}

const BOOL MOONG::DeviceInformation::get_disk_free_space_information(std::string drive, PULARGE_INTEGER freeBytesAvailableToCaller, PULARGE_INTEGER totalNumberOfBytes, PULARGE_INTEGER totalNumberOfFreeBytes)
{
	BOOL return_value = FALSE;

	if (drive.length() == 1 && ((drive.at(0) >= 'a' && drive.at(0) <= 'z') || (drive.at(0) >= 'A' && drive.at(0) <= 'Z')))
	{
		drive += ":\\";
	}

	if (GetDriveTypeA(drive.c_str()) == DRIVE_FIXED)
	{
		return_value = GetDiskFreeSpaceExA(drive.c_str(), freeBytesAvailableToCaller, totalNumberOfBytes, totalNumberOfFreeBytes);

		//std::cout << "### 테스트 HDD Space Information" << std::endl;
		//printf("freeBytesAvailableToCaller[%I64u]\n", freeBytesAvailableToCaller.QuadPart);	// 현재 사용자에게 할당된 용량 중에서 남은 용량
		//printf("totalNumberOfBytes[%I64u]\n", totalNumberOfBytes.QuadPart);
		//printf("totalNumberOfFreeBytes[%I64u]\n\n", totalNumberOfFreeBytes.QuadPart);

		//std::cout << "DRIVE [" << drive << "]" << std::endl;
		//printf("전체\t\t[%I64u]GB, [%I64u]Bytes\n", totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024), totalNumberOfBytes.QuadPart);
		//printf("사용 가능\t[%I64u]GB, [%I64u]Bytes\n", totalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024), totalNumberOfFreeBytes.QuadPart);
		//printf("사용 중\t\t[%I64u]GB, [%I64u]Bytes\n", (totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / (1024 * 1024 * 1024), totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart);
		//printf("사용량\t\t[%.2lf]%%\n", ((double)totalNumberOfBytes.QuadPart - (double)totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100.0);
		//std::cout << "### 테스트 HHDD Space Information end\n" << std::endl;
	}

	return return_value;
}