
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
		printf("���̿ռ��ܴ�С���ֽڣ� �� %I64d GB\n", dwTotalNumberOfByte / 1024 / 1024 / 1024);
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
		else {//�����Ǳ��ش���
			break;
		}
	}
	return total_size;
}

int main1(int argc, char* argv[])
{
	printf("Ӳ�̴�С: %I64dGB\n", GetDisksInformationEx());

	while (1);
	return 0;
}


int main2(void)
{
	for (int i = 0;; i++)
	{
		GUID objGuid = { 0 };
		DWORD  dwSize = 0;
		//'��ȡ����Ϊ"Ports"��GUID
		if (SetupDiClassGuidsFromName("DiskDrive", &objGuid, 1, &dwSize) == FALSE)
		{
			int a = GetLastError();
			return 0;
		}

		//'���ݴ���GUID��ȡ�豸���
		HDEVINFO 	hDevInfo = SetupDiGetClassDevs(&objGuid, 0, 0, DIGCF_PRESENT | DIGCF_PROFILE);
		if (INVALID_HANDLE_VALUE == hDevInfo)
		{
			int a = GetLastError();
			return 0;
		}

		//'�����豸�����ٰ������豸
		SP_DEVINFO_DATA  objSpdd = { 0 };
		objSpdd.cbSize = sizeof(objSpdd);

		if (SetupDiEnumDeviceInfo(hDevInfo, i, &objSpdd) == FALSE)
		{
			int a = GetLastError();
			return 0;
		}

		// ����dwIndex�豸�������FRIENDLYNAME����
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
	�밴���������. . .
	*/

	getchar();
	system("pause");
	return 0;
}


/* ************************************
* BOOL GetDiskSpaceInfo(LPCSTR pszDrive
* ���� �������������������ȡ����������
*    ���пռ䡢�������ȴ�����Ϣ
* ���� ��������·�������硰D:\����
**************************************/
BOOL GetDiskSpaceInfo(LPCSTR pszDrive)
{
	DWORD64 qwFreeBytesToCaller, qwTotalBytes, qwFreeBytes;
	DWORD dwSectPerClust, dwBytesPerSect, dwFreeClusters, dwTotalClusters;
	BOOL bResult;

	//ʹ��GetDiskFreeSpaceEx��ȡ������Ϣ����ӡ���
	bResult = GetDiskFreeSpaceExA(pszDrive,
		(PULARGE_INTEGER)&qwFreeBytesToCaller,
		(PULARGE_INTEGER)&qwTotalBytes,
		(PULARGE_INTEGER)&qwFreeBytes);

	if (bResult)
	{
		printf("ʹ��GetDiskFreeSpaceEx��ȡ���̿ռ���Ϣ\n");
		printf("�ɻ�õĿ��пռ䣨�ֽڣ�: \t%I64d\n", qwFreeBytesToCaller);
		printf("���пռ䣨�ֽڣ�: \t\t%I64d\n", qwFreeBytes);
		printf("�������������ֽڣ�: \t\t%I64d\n", qwTotalBytes);
	}

	//ʹ��GetDiskFreeSpace��ȡ������Ϣ����ӡ���
	bResult = GetDiskFreeSpaceA(pszDrive,
		&dwSectPerClust,
		&dwBytesPerSect,
		&dwFreeClusters,
		&dwTotalClusters);

	if (bResult)
	{
		printf("\nʹ��GetDiskFreeSpace��ȡ���̿ռ���Ϣ\n");
		printf("���еĴ����� : \t\t\t%d\n", dwFreeClusters);
		printf("�ܴ����� : \t\t\t%d\n", dwTotalClusters);
		printf("ÿ�ص��������� : \t\t%d\n", dwSectPerClust);
		printf("ÿ�������������ֽڣ�: \t\t%d\n", dwBytesPerSect);
		printf("���пռ䣨�ֽڣ�: \t\t%I64d\n",
			(DWORD64)dwFreeClusters*
			(DWORD64)dwSectPerClust*(DWORD64)dwBytesPerSect);
		printf("�������������ֽڣ�: \t\t%I64d",
			(DWORD64)dwTotalClusters*
			(DWORD64)dwSectPerClust*(DWORD64)dwBytesPerSect);
	}
	return bResult;
}

/* ************************************
* int main( int argc, PCHAR argv[] )
* ���� Ӧ�ó����������������������
*    ����GetDiskSpaceInfo������ȡ
*    ���̿ռ���Ϣ
* ���� ��������·�������硰D:\����
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
	//����MSDN��˵���������������2Gһ��Ҫ��GetDiskFreeSpaceEx

	printf("���: %s\n��������: %s\n�����к�: %04X-%04X\n",
		szVolumeName,
		szFileSystem,
		serialNumber >> 16,
		serialNumber & 0xFFFF
	);
	printf("�����ܿռ��С: %d MB\n", lpTotalNumberOfBytes.QuadPart / (1024 * 1024));
	printf("����ʣ��ռ��С: %d MB\n", lpTotalNumberOfFreeBytes.QuadPart / (1024 * 1024));
	printf("�����߿��õĿռ��С: %d MB\n", lpFreeBytesAvailableToCaller.QuadPart / (1024 * 1024));
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