#include "DeviceInformation.h"

#include <string>
#include <strsafe.h>

const std::string MOONG::DeviceInformation::getHDDSerial()
{
	// CMD창에서 wmic path win32_physicalmedia get serialnumber

	// Reg에서 값을 얻어오는 방법 (되도록 레지에서 해결)
	// 문제가 생길경우 아래 주석처리한 코드 참고 (권한 문제 확인 필요)
	DWORD dwSize = 0;
	LONG lStatus = 0;
	char szKeyInfo[MAX_PATH] = { 0 };
	const std::string szSubKey = "HARDWARE\\DEVICEMAP\\Scsi";
	std::string ErrorMsg;
	std::string returnVal;
	std::string tempSubKey;

	const std::string STR_SCSI_PORT = "scsi port";
	const std::string STR_SCSI_BUS = "scsi bus";
	const std::string STR_TARGET_ID = "target id";

	std::list<std::string> listScsiPort;

	if (MOONG::DeviceInformation::GetRegSubKeys(HKEY_LOCAL_MACHINE, szSubKey, listScsiPort) == EXIT_SUCCESS)
	{
		for (std::list<std::string>::iterator iterScsiPort = listScsiPort.begin(); iterScsiPort != listScsiPort.end(); ++iterScsiPort)
		{
			tempSubKey = *iterScsiPort;
			std::transform(tempSubKey.begin(), tempSubKey.end(), tempSubKey.begin(), tolower);

			if ((tempSubKey).find(STR_SCSI_PORT) != std::string::npos)
			{
				std::string strSubKeyScsiPort = szSubKey;
				strSubKeyScsiPort += "\\";
				strSubKeyScsiPort += *iterScsiPort;

				std::list<std::string> listScsiBus;

				if (MOONG::DeviceInformation::GetRegSubKeys(HKEY_LOCAL_MACHINE, strSubKeyScsiPort, listScsiBus) == EXIT_SUCCESS)
				{
					for (std::list<std::string>::iterator iterScsiBus = listScsiBus.begin(); iterScsiBus != listScsiBus.end(); ++iterScsiBus)
					{
						//std::cout << *iterScsiBus << std::endl;
						tempSubKey = *iterScsiBus;
						std::transform(tempSubKey.begin(), tempSubKey.end(), tempSubKey.begin(), tolower);

						if ((tempSubKey).find(STR_SCSI_BUS) != std::string::npos)
						{
							std::string strSubKeyScsiBus = strSubKeyScsiPort;
							strSubKeyScsiBus += "\\";
							strSubKeyScsiBus += *iterScsiBus;

							//std::cout << strSubKeyScsiBus << std::endl;

							std::list<std::string> listTargetId;

							if (MOONG::DeviceInformation::GetRegSubKeys(HKEY_LOCAL_MACHINE, strSubKeyScsiBus, listTargetId) == EXIT_SUCCESS)
							{
								for (std::list<std::string>::iterator iterTargetId = listTargetId.begin(); iterTargetId != listTargetId.end(); ++iterTargetId)
								{
									//std::cout << *iterTargetId << std::endl;
									tempSubKey = *iterTargetId;
									std::transform(tempSubKey.begin(), tempSubKey.end(), tempSubKey.begin(), tolower);

									if ((tempSubKey).find(STR_TARGET_ID) != std::string::npos)
									{
										std::string strSubKeyTargetId = strSubKeyScsiBus;
										strSubKeyTargetId += "\\";
										strSubKeyTargetId += *iterTargetId;

										//std::cout << strSubKeyTargetId << std::endl;

										HKEY hItem = NULL;
										std::string strSubKey;
										strSubKey = strSubKeyTargetId;
										strSubKey += "\\Logical Unit Id 0";

										if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0, KEY_READ, &hItem) != ERROR_SUCCESS)
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
											// 추가적으로 얻어진 키에대한 default정보 기본값을 얻는 방법이다.
											ZeroMemory(szKeyInfo, MAX_PATH);
											dwSize = sizeof(szKeyInfo);
											lStatus = RegQueryValueExA(hItem, "SerialNumber", NULL, NULL, (LPBYTE)szKeyInfo, &dwSize);
											// 만약 RegQueryValueEx()로 특정 문자열값 혹은 바이너리 값을 얻으려면 두 번째 인자에 
											// 얻고자 하는 데이터의 이름을 넣어주면된다^^
											if (lStatus == ERROR_SUCCESS)
											{
												//std::cout << szKeyInfo << std::endl;

												std::string strTemp(szKeyInfo);
												strTemp = trim(strTemp);

												if (strTemp.length() > 0)
												{
													returnVal += strTemp;
													returnVal += ";";
												}

												//break;
											}
										}
										RegCloseKey(hItem);
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
	HKEY hKey;

	DWORD dwType = REG_SZ;
	DWORD dwSize = 128;
	long lResult = 0;
	char szProcessorName[128] = { 0 };
	std::string strProcessorName;

	lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (lResult == ERROR_SUCCESS)
	{
		RegQueryValueExA(hKey, "ProcessorNameString", NULL, &dwType, (LPBYTE)szProcessorName, &dwSize);
	}

	//std::cout << "### 테스트 CPU Info" << std::endl;
	//std::cout << szBuffer << std::endl;
	//std::cout << "### 테스트 CPU Info end\n" << std::endl;

	strProcessorName = szProcessorName;

	return strProcessorName;
}

const std::string MOONG::DeviceInformation::getRAMSize()
{
	ULONGLONG ramSize;
	std::string strRamSize;

	if (!GetPhysicallyInstalledSystemMemory(&ramSize))
	{
		std::cout << "뭔가가 잘못되었다.\nMSDN 참고하고 GetLastError()를 확인해보자." << std::endl;
	}

	//std::cout << "### 테스트 RAM Size" << std::endl;
	//std::cout << ramSize << " KB" << std::endl;
	//std::cout << ramSize / 1024 << " MB" << std::endl;
	//std::cout << ramSize / 1024 / 1024 << " GB" << std::endl;
	//std::cout << "### 테스트 RAM Size end\n" << std::endl;

	ramSize /= 1024;

	strRamSize = std::to_string(ramSize);

	strRamSize += "MB";

	return strRamSize;
}

const std::string MOONG::DeviceInformation::getHDDSize()
{
	ULARGE_INTEGER freeBytesAvailableToCaller;
	ULARGE_INTEGER totalNumberOfBytes;
	ULARGE_INTEGER totalNumberOfFreeBytes;
	char driveChar[256] = { 0 };
	UINT driveType = 0;
	std::string strHDDSizeInfo;

	int count = GetLogicalDriveStringsA(256, driveChar);

	for (int i = 0; i < count / 4; i++)
	{
		driveType = GetDriveTypeA(driveChar + i * 4);

		if (driveType == DRIVE_FIXED)
		{
			GetDiskFreeSpaceExA(driveChar + i * 4, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);

			//std::cout << "### 테스트 HDD Size" << std::endl;
			////printf("%I64u\n", freeBytesAvailableToCaller);	// 현재 사용자에게 할당된 용량 중에서 남은 용량
			////printf("%I64u\n", totalNumberOfBytes);
			////printf("%I64u\n", totalNumberOfFreeBytes);

			//std::cout << "DRIVE [" << driveChar + i * 4 << "]" << std::endl;
			//printf("전체\t\t[%I64u]GB, [%I64u]Bytes\n", totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024), totalNumberOfBytes);
			//printf("사용 가능\t[%I64u]GB, [%I64u]Bytes\n", totalNumberOfFreeBytes.QuadPart / (1024 * 1024 * 1024), totalNumberOfFreeBytes);
			//printf("사용 중\t\t[%I64u]GB, [%I64u]Bytes\n", (totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / (1024 * 1024 * 1024), totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart);
			//printf("사용량\t\t[%.2lf]%%\n", ((double)totalNumberOfBytes.QuadPart - (double)totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100.0);
			//std::cout << "### 테스트 HDD Size end\n" << std::endl;

			//strHDDSizeInfo += driveChar + i * 4;
			//strHDDSizeInfo += "|";
			strHDDSizeInfo += std::to_string(totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024));		// (1000 * 1000 * 1000)로 나눠달라는 고객사 요청
			strHDDSizeInfo += "GB";
			strHDDSizeInfo += ";";
			char hddUsage[16] = { 0 };
			StringCchPrintfA(hddUsage, 16, "%.2lf", ((double)totalNumberOfBytes.QuadPart - (double)totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100.0);
			strHDDSizeInfo += hddUsage;	// 사용량 (%)
			//strHDDSizeInfo += ";";

			break;
		}
	}

	return strHDDSizeInfo;
}

const int MOONG::DeviceInformation::GetRegSubKeys(const HKEY hKey, const std::string subKey, std::list<std::string>& subKeys)
{
	HKEY hkResult = NULL;
	LONG lStatus = 0;
	DWORD dwIndex = 0;
	std::string szKeyName;
	DWORD cbName = MAX_PATH;
	int returnValue = EXIT_FAILURE;

	if (RegOpenKeyExA(hKey, subKey.c_str(), 0, KEY_READ, &hkResult) != ERROR_SUCCESS)
	{
		returnValue = EXIT_FAILURE;
	}
	else
	{
		while ((lStatus = RegEnumKeyExA(hkResult,
			dwIndex,
			(LPSTR)(szKeyName.c_str()),
			&cbName,
			NULL,
			NULL,
			NULL,
			NULL)) != ERROR_NO_MORE_ITEMS)
		{
			if (lStatus == ERROR_SUCCESS)
			{
				HKEY hItem = NULL;
				subKeys.push_back(szKeyName.c_str());

				returnValue = EXIT_SUCCESS;
			}

			dwIndex++;
			cbName = MAX_PATH;
		}

		RegCloseKey(hkResult);
	}

	return returnValue;
}