#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <objbase.h>
#include <WinBase.h>
#include <SetupAPI.h>	//setupapi.lib
//#include "public.h"

#define INTERFACE_DETAIL_SIZE   (1024)
#define WTDRIVER_MEMORY_WRITE_BAR1 IOCTL_DISK_GET_DRIVE_GEOMETRY
#define WTDRIVER_MEMORY_READ_BAR1 IOCTL_DISK_GET_DRIVE_GEOMETRY

int main()
{
	HANDLE hDevice = NULL;
	HDEVINFO hDevInfo = NULL;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail = NULL;
	GUID guid = { 0x57415445, 0x5254, 0x454b, 0x44, 0x52, 0x49, 0x56, 0x45, 0x52, 0x43, 0x24 };
	INT nCount, i;
	BOOL bResult;
	CHAR devPath[MAX_PATH];
	SP_DEVICE_INTERFACE_DATA ifdata;
	DWORD outBuffer[512] = { 0 };
	DWORD inBuffer[512];
	DWORD returnBytes = 0;


	memset(inBuffer, 1, 512);
	hDevInfo = SetupDiGetClassDevs(
		&guid,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
	);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		printf("Error: SetupDigetClassDevs .\n");
		return -1;
	}
	printf("OK --- SetupDiGetClassDevs.\n");

	// 申请设备接口数据空间   
	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);

	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	bResult = TRUE;

	// 设备序号=0,1,2... 逐一测试设备接口，到失败为止   
	while (bResult)
	{
		ifdata.cbSize = sizeof(ifdata);

		// 枚举符合该GUID的设备接口   
		bResult = SetupDiEnumDeviceInterfaces(
			hDevInfo,            // 设备信息集句柄   
			NULL,                   // 不需额外的设备描述   
			&guid,                 // GUID   
			(ULONG)nCount,          // 设备信息集里的设备序号   
			&ifdata);               // 设备接口信息   

		if (bResult)
		{
			// 取得该设备接口的细节(设备路径)   
			bResult = SetupDiGetInterfaceDeviceDetail(
				hDevInfo,            // 设备信息集句柄   
				&ifdata,                // 设备接口信息   
				pDetail,                // 设备接口细节(设备路径)   
				INTERFACE_DETAIL_SIZE,  // 输出缓冲区大小   
				NULL,                   // 不需计算输出缓冲区大小(直接用设定值)   
				NULL);                  // 不需额外的设备描述   

			if (bResult)
			{
				strcpy(devPath, pDetail->DevicePath);
				printf("lenth = %d\nDevicePath: %s\n\n", strlen(devPath), devPath);
				nCount++;
			}
		}
		else
		{
			printf("error: %d\n", GetLastError());
		}
	}

	hDevice = CreateFile(
		devPath,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Error: CreateFile. code = %d\n", GetLastError);
		return -2;
	}
	printf("OK --- CreateFile.\n");


	printf("Bytes = %d,  outBuffer; %s\n", returnBytes, outBuffer);


	bResult = DeviceIoControl(
		hDevice,
		WTDRIVER_MEMORY_WRITE_BAR1,
		inBuffer, 256,
		NULL, 0,
		&returnBytes,
		NULL
	);
	if (!bResult) {
		printf("Error: DeviceIoControl.");
		return -3;
	}

	bResult = DeviceIoControl(
		hDevice,														//设备句柄
		WTDRIVER_MEMORY_READ_BAR1,										//读BAR1
		NULL, 0,
		outBuffer, 256,													//读在outBuffer里，读256个字节
		&returnBytes,													//实际读取字节数
		NULL
	);
	if (!bResult) {
		printf("Error: DeviceIoControl.");
		return -3;
	}

	printf("Bytes = %d\n", returnBytes);
	for (i = 0; i < returnBytes; i++)
	{
		printf("%d: outBuffer: %#x\n", i, outBuffer[i]);
	}

	CloseHandle(hDevice);
	// 释放设备接口数据空间   
	GlobalFree(pDetail);
	// 关闭设备信息集句柄   
	SetupDiDestroyDeviceInfoList(hDevInfo);


	system("pause");
	return 0;
}
