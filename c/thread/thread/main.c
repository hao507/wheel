#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "thread.h"

void* func(void *arg)
{
	while (1)
	{
		printf("this is thread is running...\n");
		Sleep(1000);
	}

	return NULL;
}

int main()
{
	thread_t t;
	thread_create(&t, func, NULL, 1024);

	system("pause");
	return EXIT_SUCCESS;
}
