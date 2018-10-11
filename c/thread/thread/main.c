#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "thread.h"

int num = 0;

thread_mutex_t mutex;

void* func(void *arg)
{
	int i = 0;
	while (++i < 5)
	{
		printf("this is thread is running...\n");
		Sleep(1000);
	}

	thread_exit(NULL);

	return NULL;
}

void* func2(void *arg)
{
	for (int i = 0; i < 100; i++)
	{
		thread_mutex_lock(mutex);
		printf("func2 before:%d\n", num);
		num = i;
		printf("func2 after:%d\n", num);
		thread_mutex_unlock(mutex);
		thread_sleep(1000);
	}

	return NULL;
}

int main()
{
	int ret;
	ret = thread_mutex_init(&mutex);
	if (ret != 0)
	{
		printf("mutex init error\n");
		return 1;
	}

	thread_t t;
	thread_create(&t, func2, NULL, 1024);

	for (int i = 100; i < 1000; i++)
	{
		thread_mutex_lock(mutex);
		printf("main before:%d\n", num);
		num = i;
		printf("main after:%d\n", num);
		thread_mutex_unlock(mutex);
		thread_sleep(1000);
	}

	system("pause");
	return EXIT_SUCCESS;
}
