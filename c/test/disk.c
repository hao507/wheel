#include <stdio.h>
#include <Windows.h>


void getType(char *);
int main00(int argc, char** args)
{
	HANDLE h = GetStdHandle(-1);
	if (h == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		printf("err:%d\n", err);
	}

	system("pause");

	return 0;

	char dwPath[MAX_PATH];
	char* p = dwPath;

	GetLogicalDriveStrings(MAX_PATH - 1, dwPath);
	int s = strlen(dwPath);
	printf("s = %d\n", s);
	do
	{

		CHAR volumeNmaeBuff[MAX_PATH];
		DWORD volumeSerialNumber;
		DWORD maximumComponentLength;
		DWORD fileSystemFlag;
		CHAR fileSystemName[MAX_PATH];

		//getType(p);

		if (GetVolumeInformation(p,
			volumeNmaeBuff,
			MAX_PATH,
			&volumeSerialNumber,
			&maximumComponentLength,
			&fileSystemFlag,
			fileSystemName,
			MAX_PATH))
		{
			printf("\r\n==========================================================\r\n");
			printf("%s  \r\n", p);
			if (strlen(volumeNmaeBuff) != 0)
			{
				printf("��������꣺%s\r\n", volumeNmaeBuff);
			}
			printf("���������кţ�%lu\r\n\
����ļ������ȣ�%u\r\n\
�ļ�ϵͳ��ʶ��%u\r\n\
�ļ�ϵͳ���ͣ�%s\r\n", volumeSerialNumber, maximumComponentLength, fileSystemFlag, fileSystemName);

		}
		else
		{

			return FALSE;
		}


		p += (s + 1);


	} while (*p != '\x00');


	system("pause");
	return 0;
}

void getType(char * str)
{

	UINT uTYPE;
	uTYPE = GetDriveType(str);
	switch (uTYPE)
	{

	case DRIVE_UNKNOWN:
		printf("%s  δ֪��������\r\n", str);
		break;
	case DRIVE_NO_ROOT_DIR:
		printf("%s  ��Ч�̷�\r\n", str);
		break;
	case DRIVE_REMOVABLE:
		printf("%s  �ƶ�����\r\n", str);
		break;
	case DRIVE_FIXED:
		printf("%s  �̶�����\r\n", str);
		break;
	case DRIVE_REMOTE:
		printf("%s  �������\r\n", str);
		break;
	case DRIVE_CDROM:
		printf("%s  ����\r\n", str);
		break;
	case DRIVE_RAMDISK:
		printf("%s  RAM����\r\n", str);
	default:
		break;
	}
}

BOOL bDriver[27];
BOOL bFirst = TRUE;
void dect()
{
	char szDriver[3];
	for (int i = 'C'; i <= 'Z'; i++)
	{
		szDriver[0] = i;
		szDriver[1] = ':';
		szDriver[2] = 0;
		UINT uType = GetDriveType(szDriver);
		if (uType == DRIVE_REMOVABLE || uType == DRIVE_FIXED)
		{
			if ((!bFirst) && (!bDriver[i - 'C'])) printf("found %c:\n", i);
			bDriver[i - 'C'] = TRUE;
		}
		else //����?
		{
			if (bDriver[i - 'C'])
			{
				bDriver[i - 'C'] = FALSE;
				printf("remove %c:\n", i);
			}
		}
	}
	if (bFirst) bFirst = FALSE;
}

int main01()
{
	ZeroMemory(bDriver, sizeof(bDriver));
	while (1)
	{
		dect();
		Sleep(2000);
	}
	return 0;
}