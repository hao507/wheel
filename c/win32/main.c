#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <Windows.h>

int main()
{
	HANDLE h = GetStdHandle(100);
	if (h == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		printf("err:%d\n", err);
	}

	system("pause");
	return EXIT_SUCCESS;
}
