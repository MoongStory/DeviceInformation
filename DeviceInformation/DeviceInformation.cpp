#include "DeviceInformation.h"
#include "../../Registry/Registry/Registry.h" // https://github.com/MoongStory/Registry

#include <strsafe.h>

const std::string MOONG::DeviceInformation::getHDDSerial()
{
	// CMD창에서 wmic path win32_physicalmedia get serialnumber

	// Reg에서 값을 얻어오는 방법 (되도록 레지에서 해결)
	const std::string szSubKey = "HARDWARE\\DEVICEMAP\\Scsi";
	std::string ErrorMsg;
	std::string returnVal;
	std::string tempSubKey;
	std::string hdd_serial_number;

	std::list<std::string> listScsiPort;

	if (MOONG::Registry::getRegSubKeys(HKEY_LOCAL_MACHINE, szSubKey, listScsiPort) == EXIT_SUCCESS)
	{
		for (std::list<std::string>::iterator iterScsiPort = listScsiPort.begin(); iterScsiPort != listScsiPort.end(); ++iterScsiPort)
		{
			tempSubKey = *iterScsiPort;
			std::transform(tempSubKey.begin(), tempSubKey.end(), tempSubKey.begin(), tolower);

			if ((tempSubKey).find("scsi port") != std::string::npos)
			{
				std::string strSubKeyScsiPort = szSubKey;
				strSubKeyScsiPort += "\\";
				strSubKeyScsiPort += *iterScsiPort;

				std::list<std::string> listScsiBus;

				if (MOONG::Registry::getRegSubKeys(HKEY_LOCAL_MACHINE, strSubKeyScsiPort, listScsiBus) == EXIT_SUCCESS)
				{
					for (std::list<std::string>::iterator iterScsiBus = listScsiBus.begin(); iterScsiBus != listScsiBus.end(); ++iterScsiBus)
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

							std::list<std::string> listTargetId;

							if (MOONG::Registry::getRegSubKeys(HKEY_LOCAL_MACHINE, strSubKeyScsiBus, listTargetId) == EXIT_SUCCESS)
							{
								for (std::list<std::string>::iterator iterTargetId = listTargetId.begin(); iterTargetId != listTargetId.end(); ++iterTargetId)
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
											ErrorMsg += "Can't open subkey [";
											ErrorMsg += strSubKey;
											ErrorMsg += "]";

											//std::cout << ErrorMsg.c_str() << std::endl;

											ErrorMsg.clear();

											continue;
										}
										else
										{
											hdd_serial_number = trim(hdd_serial_number);

											//std::cout << output << std::endl;

											if (hdd_serial_number.length() > 0)
											{
												returnVal += hdd_serial_number;
												returnVal += ";";
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

	return returnVal;

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

const std::string MOONG::DeviceInformation::getProcessorInformation()
{
	std::string processor_information;

	MOONG::Registry::Read(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", processor_information);

	return processor_information;
}

const ULONGLONG MOONG::DeviceInformation::getRAMSize()
{
	ULONGLONG ram_size = 0;

	if (!GetPhysicallyInstalledSystemMemory(&ram_size))
	{
		std::cout << "뭔가가 잘못되었다.\nMSDN 참고하고 GetLastError() 확인." << std::endl;
	}

	//std::cout << "\n### 테스트 RAM Size" << std::endl;
	//std::cout << ramSize << " KB" << std::endl;
	//std::cout << ramSize / 1024 << " MB" << std::endl;
	//std::cout << ramSize / 1024 / 1024 << " GB" << std::endl;
	//std::cout << "### 테스트 RAM Size end\n" << std::endl;

	return ram_size;
}

const ULONGLONG MOONG::DeviceInformation::getHDDTotalSize(std::string drive)
{
	ULARGE_INTEGER freeBytesAvailableToCaller = { 0 };
	ULARGE_INTEGER totalNumberOfBytes = { 0 };
	ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };

	MOONG::DeviceInformation::GetDiskFreeSpaceInformation(drive, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);

	return totalNumberOfBytes.QuadPart;
}

const ULONGLONG MOONG::DeviceInformation::getHDDAvailableSize(std::string drive)
{
	ULARGE_INTEGER freeBytesAvailableToCaller = { 0 };
	ULARGE_INTEGER totalNumberOfBytes = { 0 };
	ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };

	MOONG::DeviceInformation::GetDiskFreeSpaceInformation(drive, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);

	return totalNumberOfFreeBytes.QuadPart;
}

const ULONGLONG MOONG::DeviceInformation::getHDDUsingSize(std::string drive)
{
	ULARGE_INTEGER freeBytesAvailableToCaller = { 0 };
	ULARGE_INTEGER totalNumberOfBytes = { 0 };
	ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };

	MOONG::DeviceInformation::GetDiskFreeSpaceInformation(drive, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);

	return totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart;
}

const double MOONG::DeviceInformation::getHDDUsage(std::string drive)
{
	ULARGE_INTEGER freeBytesAvailableToCaller = { 0 };
	ULARGE_INTEGER totalNumberOfBytes = { 0 };
	ULARGE_INTEGER totalNumberOfFreeBytes = { 0 };

	double hdd_usage = 0;

	if (MOONG::DeviceInformation::GetDiskFreeSpaceInformation(drive, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes) == TRUE)
	{
		hdd_usage = ((double)totalNumberOfBytes.QuadPart - (double)totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100.0;
	}

	return hdd_usage;
}

const BOOL MOONG::DeviceInformation::GetDiskFreeSpaceInformation(std::string drive, PULARGE_INTEGER freeBytesAvailableToCaller, PULARGE_INTEGER totalNumberOfBytes, PULARGE_INTEGER totalNumberOfFreeBytes)
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