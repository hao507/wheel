#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <Windows.h>
#include <WinSock2.h>

void test()
{
	printf("exit ....\n");
}

int main()
{
	atexit(test);


	system("pause");
	return EXIT_SUCCESS;
}
