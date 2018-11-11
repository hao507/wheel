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

	// �����豸�ӿ����ݿռ�   
	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);

	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	bResult = TRUE;

	// �豸���=0,1,2... ��һ�����豸�ӿڣ���ʧ��Ϊֹ   
	while (bResult)
	{
		ifdata.cbSize = sizeof(ifdata);

		// ö�ٷ��ϸ�GUID���豸�ӿ�   
		bResult = SetupDiEnumDeviceInterfaces(
			hDevInfo,            // �豸��Ϣ�����   
			NULL,                   // ���������豸����   
			&guid,                 // GUID   
			(ULONG)nCount,          // �豸��Ϣ������豸���   
			&ifdata);               // �豸�ӿ���Ϣ   

		if (bResult)
		{
			// ȡ�ø��豸�ӿڵ�ϸ��(�豸·��)   
			bResult = SetupDiGetInterfaceDeviceDetail(
				hDevInfo,            // �豸��Ϣ�����   
				&ifdata,                // �豸�ӿ���Ϣ   
				pDetail,                // �豸�ӿ�ϸ��(�豸·��)   
				INTERFACE_DETAIL_SIZE,  // �����������С   
				NULL,                   // ������������������С(ֱ�����趨ֵ)   
				NULL);                  // ���������豸����   

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
		hDevice,														//�豸���
		WTDRIVER_MEMORY_READ_BAR1,										//��BAR1
		NULL, 0,
		outBuffer, 256,													//����outBuffer���256���ֽ�
		&returnBytes,													//ʵ�ʶ�ȡ�ֽ���
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
	// �ͷ��豸�ӿ����ݿռ�   
	GlobalFree(pDetail);
	// �ر��豸��Ϣ�����   
	SetupDiDestroyDeviceInfoList(hDevInfo);


	system("pause");
	return 0;
}
