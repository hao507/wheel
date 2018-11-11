
#include <windows.h>
#include <stdio.h>
#include <initguid.h>
#include <setupapi.h>
#include <WMIUtils.h>
#pragma comment(lib,"setupapi.lib")

// SysInfo.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#define BUFFERSIZE 255
long GetDiskSpaceInformation(LPSTR szDrive)
{
	//  printf("volume  %s \n",szDrive);

	DWORD64 dwAvailableByte, dwTotalNumberOfByte = 0, dwFreeNumberOfByte;
	if (!GetDiskFreeSpaceEx(szDrive, (PULARGE_INTEGER)&dwAvailableByte,
		(PULARGE_INTEGER)&dwTotalNumberOfByte, (PULARGE_INTEGER)&dwFreeNumberOfByte))
	{
		printf("cannot get disk free space \n");
	}
	else
	{
		printf("磁盘空间总大小（字节） ： %I64d GB\n", dwTotalNumberOfByte / 1024 / 1024 / 1024);
	}
	return dwTotalNumberOfByte / 1024 / 1024 / 1024;
}

long GetDisksInformationEx()
{
	long total_size = 0;
	printf("Begin Call GetDisksInformationEx()\n");
	CHAR szLogicalDriveString[BUFFERSIZE] = { 0 };
	PCHAR szDrive;

	GetLogicalDriveStrings(BUFFERSIZE - 1, szLogicalDriveString);
	szDrive = szLogicalDriveString;
	while (*szDrive)
	{
		if (GetDriveType(szDrive) == DRIVE_FIXED) {
			printf("volume: %s\n", szDrive);
			long size = GetDiskSpaceInformation(szDrive);
			total_size += size;
			szDrive += (lstrlen(szDrive) + 1);
		}
		else {//读到非本地磁盘
			break;
		}
	}
	return total_size;
}

int main1(int argc, char* argv[])
{
	printf("硬盘大小: %I64dGB\n", GetDisksInformationEx());

	while (1);
	return 0;
}


int main2(void)
{
	for (int i = 0;; i++)
	{
		GUID objGuid = { 0 };
		DWORD  dwSize = 0;
		//'获取类名为"Ports"的GUID
		if (SetupDiClassGuidsFromName("DiskDrive", &objGuid, 1, &dwSize) == FALSE)
		{
			int a = GetLastError();
			return 0;
		}

		//'根据串口GUID获取设备句柄
		HDEVINFO 	hDevInfo = SetupDiGetClassDevs(&objGuid, 0, 0, DIGCF_PRESENT | DIGCF_PROFILE);
		if (INVALID_HANDLE_VALUE == hDevInfo)
		{
			int a = GetLastError();
			return 0;
		}

		//'根据设备句柄检举包含的设备
		SP_DEVINFO_DATA  objSpdd = { 0 };
		objSpdd.cbSize = sizeof(objSpdd);

		if (SetupDiEnumDeviceInfo(hDevInfo, i, &objSpdd) == FALSE)
		{
			int a = GetLastError();
			return 0;
		}

		// 根据dwIndex设备句柄请求FRIENDLYNAME访问
		BYTE PropertyBuffer[MAX_PATH] = { 0 };
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &objSpdd, SPDRP_FRIENDLYNAME, 0, PropertyBuffer, MAX_PATH, &dwSize) == FALSE)
		{
			int a = GetLastError();
			return 0;
		}
		printf("%s\n", PropertyBuffer);

	}
	/*
	ST1000DM003-1SB10C
	INTEL SSDSC2BW240H6
	INTEL SSDSC2KW120H6
	TOSHIBA USB FLASH DRIVE USB Device
	请按任意键继续. . .
	*/

	getchar();
	system("pause");
	return 0;
}


/* ************************************
* BOOL GetDiskSpaceInfo(LPCSTR pszDrive
* 功能 根据输入的驱动器，获取磁盘总容量
*    空闲空间、簇数量等磁盘信息
* 参数 驱动器根路径，比如“D:\”。
**************************************/
BOOL GetDiskSpaceInfo(LPCSTR pszDrive)
{
	DWORD64 qwFreeBytesToCaller, qwTotalBytes, qwFreeBytes;
	DWORD dwSectPerClust, dwBytesPerSect, dwFreeClusters, dwTotalClusters;
	BOOL bResult;

	//使用GetDiskFreeSpaceEx获取磁盘信息并打印结果
	bResult = GetDiskFreeSpaceExA(pszDrive,
		(PULARGE_INTEGER)&qwFreeBytesToCaller,
		(PULARGE_INTEGER)&qwTotalBytes,
		(PULARGE_INTEGER)&qwFreeBytes);

	if (bResult)
	{
		printf("使用GetDiskFreeSpaceEx获取磁盘空间信息\n");
		printf("可获得的空闲空间（字节）: \t%I64d\n", qwFreeBytesToCaller);
		printf("空闲空间（字节）: \t\t%I64d\n", qwFreeBytes);
		printf("磁盘总容量（字节）: \t\t%I64d\n", qwTotalBytes);
	}

	//使用GetDiskFreeSpace获取磁盘信息并打印结果
	bResult = GetDiskFreeSpaceA(pszDrive,
		&dwSectPerClust,
		&dwBytesPerSect,
		&dwFreeClusters,
		&dwTotalClusters);

	if (bResult)
	{
		printf("\n使用GetDiskFreeSpace获取磁盘空间信息\n");
		printf("空闲的簇数量 : \t\t\t%d\n", dwFreeClusters);
		printf("总簇数量 : \t\t\t%d\n", dwTotalClusters);
		printf("每簇的扇区数量 : \t\t%d\n", dwSectPerClust);
		printf("每扇区的容量（字节）: \t\t%d\n", dwBytesPerSect);
		printf("空闲空间（字节）: \t\t%I64d\n",
			(DWORD64)dwFreeClusters*
			(DWORD64)dwSectPerClust*(DWORD64)dwBytesPerSect);
		printf("磁盘总容量（字节）: \t\t%I64d",
			(DWORD64)dwTotalClusters*
			(DWORD64)dwSectPerClust*(DWORD64)dwBytesPerSect);
	}
	return bResult;
}

/* ************************************
* int main( int argc, PCHAR argv[] )
* 功能 应用程序主函数，根据输入参数
*    调用GetDiskSpaceInfo函数获取
*    磁盘空间信息
* 参数 驱动器根路径，比如“D:\”。
**************************************/
int main3(int argc, PCHAR argv[])
{
	GetDiskSpaceInfo("C:");
	getchar();
	return 0;
}

int main4() {
	_ULARGE_INTEGER lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
	DWORD serialNumber, maxComponentLength, fsFlags;
	TCHAR szFileSystem[12];
	TCHAR szVolumeName[24];
	TCHAR szRoot[4] = "c:\\";

	if (!GetVolumeInformation(
		szRoot,
		szVolumeName,
		sizeof(szVolumeName),
		&serialNumber,
		&maxComponentLength,
		&fsFlags,
		szFileSystem,
		sizeof(szFileSystem))) {
		printf("Failed to retrieve drive information\n");
	}
	GetDiskFreeSpaceEx(szRoot, &lpFreeBytesAvailableToCaller, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes);
	//根据MSDN的说明，如果分区大于2G一定要用GetDiskFreeSpaceEx

	printf("卷标: %s\n分区类型: %s\n卷序列号: %04X-%04X\n",
		szVolumeName,
		szFileSystem,
		serialNumber >> 16,
		serialNumber & 0xFFFF
	);
	printf("分区总空间大小: %d MB\n", lpTotalNumberOfBytes.QuadPart / (1024 * 1024));
	printf("分区剩余空间大小: %d MB\n", lpTotalNumberOfFreeBytes.QuadPart / (1024 * 1024));
	printf("调用者可用的空间大小: %d MB\n", lpFreeBytesAvailableToCaller.QuadPart / (1024 * 1024));
	getchar();
	return(0);
}

///******************************************************************************
//* Function: get all present disks' physical number
//* input: N/A
//* output: ppDisks, array of disks' physical number
//* return: Succeed, the amount of present disks
//*         Fail, -1
//******************************************************************************/
//DWORD GetAllPresentDisks(DWORD **ppDisks)
//{
//	SetupDiGetDeviceInterfaceDetail	CHAR *szDevicePath[MAX_DEVICE];        // device path
//	DWORD nDevice;
//	HANDLE hDevice;
//	STORAGE_DEVICE_NUMBER number;
//	BOOL result;
//	DWORD readed;
//	WORD i, j;
//
//	for (i = 0; i < MAX_DEVICE; i++)
//	{
//		szDevicePath[i] = (CHAR *)malloc(INTERFACE_DETAIL_SIZE);
//		if (NULL == szDevicePath[i])
//		{
//			for (j = 0; j < i; j++)
//			{
//				free(szDevicePath[i]);
//			}
//			return (DWORD)-1;
//		}
//	}
//
//	// get the device paths
//	nDevice = GetDevicePath(const_cast<LPGUID>(&GUID_DEVINTERFACE_DISK), szDevicePath);
//	if ((DWORD)-1 == nDevice)
//	{
//		for (i = 0; i < MAX_DEVICE; i++)
//		{
//			free(szDevicePath[i]);
//		}
//		return (DWORD)-1;
//	}
//
//	*ppDisks = (DWORD *)malloc(sizeof(DWORD) * nDevice);
//	// get the disk's physical number one by one
//	for (i = 0; i < nDevice; i++)
//	{
//		hDevice = CreateFile(
//			szDevicePath[i], // drive to open
//			GENERIC_READ | GENERIC_WRITE,     // access to the drive
//			FILE_SHARE_READ | FILE_SHARE_WRITE, //share mode
//			NULL,             // default security attributes
//			OPEN_EXISTING,    // disposition
//			0,                // file attributes
//			NULL            // do not copy file attribute
//		);
//		if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
//		{
//			for (j = 0; j < MAX_DEVICE; j++)
//			{
//				free(szDevicePath[j]);
//			}
//			free(*ppDisks);
//			fprintf(stderr, "CreateFile() Error: %ld\n", GetLastError());
//			return DWORD(-1);
//		}
//		result = DeviceIoControl(
//			hDevice,                // handle to device
//			IOCTL_STORAGE_GET_DEVICE_NUMBER, // dwIoControlCode
//			NULL,                            // lpInBuffer
//			0,                               // nInBufferSize
//			&number,           // output buffer
//			sizeof(number),         // size of output buffer
//			&readed,       // number of bytes returned
//			NULL      // OVERLAPPED structure
//		);
//		if (!result) // fail
//		{
//			fprintf(stderr, "IOCTL_STORAGE_GET_DEVICE_NUMBER Error: %ld\n", GetLastError());
//			for (j = 0; j < MAX_DEVICE; j++)
//			{
//				free(szDevicePath[j]);
//			}
//			free(*ppDisks);
//			(void)CloseHandle(hDevice);
//			return (DWORD)-1;
//		}
//		*(*ppDisks + i) = number.DeviceNumber;
//
//		(void)CloseHandle(hDevice);
//	}
//
//	for (i = 0; i < MAX_DEVICE; i++)
//	{
//		free(szDevicePath[i]);
//	}
//	return nDevice;
//}