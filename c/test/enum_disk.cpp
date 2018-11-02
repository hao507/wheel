#define _CRT_SECURE_NO_WARNINGS
// phylist1.cpp : Defines the entry point for the console application.
//

#include   <windows.h>
#include   <stdio.h>   
#include   <conio.h> 
#include   <winioctl.h>
#include   <setupapi.h>
#define	INTERFACE_DETAIL_SIZE 1024

#if 0

#define MAX_DEVICE  10
#define INTERFACE_DETAIL_SIZE    (1024)
DEFINE_GUID(GUID_DEVINTERFACE_DISK, 0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

/******************************************************************************
* Function: get device path from GUID
* input: lpGuid, GUID pointer
* output: pszDevicePath, device paths
* return: Succeed, the amount of found device paths
*         Fail, -1
******************************************************************************/
DWORD GetDevicePath(LPGUID lpGuid, WCHAR **pszDevicePath)
{
	HDEVINFO hDevInfoSet;
	SP_DEVICE_INTERFACE_DATA ifdata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
	DWORD nCount;
	BOOL result;

	//get a handle to a device information set
	hDevInfoSet = SetupDiGetClassDevs(
		lpGuid,      // class GUID
		NULL,        // Enumerator
		NULL,        // hwndParent
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE    // present devices
	);

	//fail...
	if (hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "IOCTL_STORAGE_GET_DEVICE_NUMBER Error: %ld\n", GetLastError());
		return (DWORD)-1;
	}

	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(INTERFACE_DETAIL_SIZE);
	if (pDetail == NULL)
	{
		return (DWORD)-1;
	}
	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	result = TRUE;

	// device index = 0, 1, 2... test the device interface one by one
	while (result)
	{
		ifdata.cbSize = sizeof(ifdata);

		//enumerates the device interfaces that are contained in a device information set
		result = SetupDiEnumDeviceInterfaces(
			hDevInfoSet,     // DeviceInfoSet
			NULL,            // DeviceInfoData
			lpGuid,          // GUID
			nCount,   // MemberIndex
			&ifdata        // DeviceInterfaceData
		);
		if (result)
		{
			// get details about a device interface
			result = SetupDiGetDeviceInterfaceDetail(
				hDevInfoSet,    // DeviceInfoSet
				&ifdata,        // DeviceInterfaceData
				pDetail,        // DeviceInterfaceDetailData
				INTERFACE_DETAIL_SIZE,    // DeviceInterfaceDetailDataSize
				NULL,           // RequiredSize
				NULL          // DeviceInfoData
			);
			if (result)
			{
				// copy the path to output buffer
				wcscpy(pszDevicePath[nCount], (const wchar_t*)pDetail->DevicePath);
				printf("%ws\n", pDetail->DevicePath);
				nCount++;
			}
		}
	}

	free(pDetail);
	(void)SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return nCount;
}

/******************************************************************************
* Function: get all present disks' physical number
* input: N/A
* output: ppDisks, array of disks' physical number
* return: Succeed, the amount of present disks
*         Fail, -1
******************************************************************************/
DWORD GetAllPresentDisks(DWORD **ppDisks)
{
	WCHAR *szDevicePath[MAX_DEVICE];        // device path
	DWORD nDevice;
	HANDLE hDevice;
	STORAGE_DEVICE_NUMBER number;
	BOOL result;
	DWORD readed;
	WORD i, j;

	for (i = 0; i < MAX_DEVICE; i++)
	{
		szDevicePath[i] = (WCHAR *)malloc(INTERFACE_DETAIL_SIZE);
		if (NULL == szDevicePath[i])
		{
			for (j = 0; j < i; j++)
			{
				free(szDevicePath[i]);
			}
			return (DWORD)-1;
		}
	}

	// get the device paths
	nDevice = GetDevicePath(const_cast<LPGUID>(&GUID_DEVINTERFACE_DISK), szDevicePath);
	if ((DWORD)-1 == nDevice)
	{
		for (i = 0; i < MAX_DEVICE; i++)
		{
			free(szDevicePath[i]);
		}
		return (DWORD)-1;
	}

	*ppDisks = (DWORD *)malloc(sizeof(DWORD) * nDevice);

	// get the disk's physical number one by one
	for (i = 0; i < nDevice; i++)
	{
		hDevice = CreateFile(
			(LPCSTR)szDevicePath[i], // drive to open
			GENERIC_READ | GENERIC_WRITE,     // access to the drive
			FILE_SHARE_READ | FILE_SHARE_WRITE, //share mode
			NULL,             // default security attributes
			OPEN_EXISTING,    // disposition
			0,                // file attributes
			NULL            // do not copy file attribute
		);
		if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
		{
			for (j = 0; j < MAX_DEVICE; j++)
			{
				free(szDevicePath[j]);
			}
			free(*ppDisks);
			fprintf(stderr, "CreateFile() Error: %ld\n", GetLastError());
			return DWORD(-1);
		}
		result = DeviceIoControl(
			hDevice,                // handle to device
			IOCTL_STORAGE_GET_DEVICE_NUMBER, // dwIoControlCode
			NULL,                            // lpInBuffer
			0,                               // nInBufferSize
			&number,           // output buffer
			sizeof(number),         // size of output buffer
			&readed,       // number of bytes returned
			NULL      // OVERLAPPED structure
		);
		if (!result) // fail
		{
			fprintf(stderr, "IOCTL_STORAGE_GET_DEVICE_NUMBER Error: %ld\n", GetLastError());
			for (j = 0; j < MAX_DEVICE; j++)
			{
				free(szDevicePath[j]);
			}
			free(*ppDisks);
			(void)CloseHandle(hDevice);
			return (DWORD)-1;
		}
		*(*ppDisks + i) = number.DeviceNumber;
		// printf("%d\n",number.DeviceNumber);
		(void)CloseHandle(hDevice);
	}

	for (i = 0; i < MAX_DEVICE; i++)
	{
		free(szDevicePath[i]);
	}
	return nDevice;
}

int main(int argc, const char* argv[])
{
	DWORD *pDisk;
	int t, j;
	t = GetAllPresentDisks(&pDisk);
	for (j = 0; j<t; j++)
	{
		printf("%d \n", *pDisk++);
	}

	_getch();
	return 0;
}

#endif


/******************************************************************************
* Function: get device path from GUID
* input: lpGuid, GUID pointer
* output: pszDevicePath, device paths
* return: Succeed, the amount of found device paths
*         Fail, -1
******************************************************************************/
DWORD GetDevicePath(LPGUID lpGuid, CHAR **pszDevicePath)
{
	HDEVINFO hDevInfoSet;
	SP_DEVICE_INTERFACE_DATA ifdata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
	DWORD nCount;
	BOOL result;

	//get a handle to a device information set
	hDevInfoSet = SetupDiGetClassDevs(
		lpGuid,      // class GUID
		NULL,        // Enumerator
		NULL,        // hwndParent
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE    // present devices
	);

	//fail...
	if (hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "IOCTL_STORAGE_GET_DEVICE_NUMBER Error: %ld\n", GetLastError());
		return (DWORD)-1;
	}

	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(INTERFACE_DETAIL_SIZE);
	if (pDetail == NULL)
	{
		return (DWORD)-1;
	}
	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	result = TRUE;

	// device index = 0, 1, 2... test the device interface one by one
	while (result)
	{
		ifdata.cbSize = sizeof(ifdata);

		//enumerates the device interfaces that are contained in a device information set
		result = SetupDiEnumDeviceInterfaces(
			hDevInfoSet,     // DeviceInfoSet
			NULL,            // DeviceInfoData
			lpGuid,          // GUID
			nCount,   // MemberIndex
			&ifdata        // DeviceInterfaceData
		);
		if (result)
		{
			// get details about a device interface
			result = SetupDiGetDeviceInterfaceDetail(
				hDevInfoSet,    // DeviceInfoSet
				&ifdata,        // DeviceInterfaceData
				pDetail,        // DeviceInterfaceDetailData
				INTERFACE_DETAIL_SIZE,    // DeviceInterfaceDetailDataSize
				NULL,           // RequiredSize
				NULL          // DeviceInfoData
			);
			if (result)
			{
				// copy the path to output buffer
				strcpy(pszDevicePath[nCount], pDetail->DevicePath);
				//printf("%s\n", pDetail->DevicePath);
				nCount++;
			}
		}
	}

	free(pDetail);
	(void)SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return nCount;
}
