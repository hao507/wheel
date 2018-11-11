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
				printf("驱动器卷标：%s\r\n", volumeNmaeBuff);
			}
			printf("驱动器序列号：%lu\r\n\
最大文件名长度：%u\r\n\
文件系统标识：%u\r\n\
文件系统类型：%s\r\n", volumeSerialNumber, maximumComponentLength, fileSystemFlag, fileSystemName);

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
		printf("%s  未知磁盘类型\r\n", str);
		break;
	case DRIVE_NO_ROOT_DIR:
		printf("%s  无效盘符\r\n", str);
		break;
	case DRIVE_REMOVABLE:
		printf("%s  移动磁盘\r\n", str);
		break;
	case DRIVE_FIXED:
		printf("%s  固定磁盘\r\n", str);
		break;
	case DRIVE_REMOTE:
		printf("%s  网络磁盘\r\n", str);
		break;
	case DRIVE_CDROM:
		printf("%s  光驱\r\n", str);
		break;
	case DRIVE_RAMDISK:
		printf("%s  RAM磁盘\r\n", str);
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
		else //移走?
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