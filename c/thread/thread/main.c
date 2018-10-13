#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "thread.h"

thread_mutex_t mutex;

void peixt(const char* einfo)
{
	printf("%s error, will be exit.\n", einfo);
	perror("error info:");
	thread_sleep(5000);
	exit(1);
}

void* callback(void *arg)
{
	int ret = -1;

	ret = thread_mutex_lock(&mutex);
	if (ret != 0)
		peixt("thread_mutex_lock--");

	printf("I'm callback will sleep 5s\n");
	thread_sleep(5000);

	ret = thread_mutex_unlock(&mutex);
	if (ret != 0)
		peixt("thread_mutex_unlock");

	printf("I'm callback will exit...\n");

	return NULL;
}

int main()
{
	int ret;
	ret = thread_mutex_init(&mutex);
	if (ret != 0)
		peixt("thread_mutex_init");

	thread_t t;
	ret = thread_create(&t, callback, NULL, 1024);
	if (ret != 0)
		peixt("thread_create");

	ret = thread_detach(t);
	if (ret != 0)
	{
		printf("thread_detach error:%s\n", strerror(ret));
		return 1;
	}

	//ret = thread_join(t, NULL);
	//if (ret != 0)
	//	peixt("thread_join");

	thread_sleep(1000);

	if (thread_mutex_trylock(&mutex) == 0)
	{
		printf("thread_mutex_trylock success\n");
	}
	else
	{
		printf("thread_mutex_trylock fail, try thread_mutex_lock\n");
		printf("will be thread_cancel thread after 2s\n");
		thread_sleep(2000);
		ret = thread_cancel(t);
		if (ret != 0)
			peixt("thread_cancel");

		thread_mutex_unlock(&mutex);
		ret = thread_mutex_lock(&mutex);
		if (ret != 0)
			peixt("thread_mutex_lock");

		printf("thread_mutex_lock success, I'm main, will sleep 1s\n");
		thread_sleep(1000);

		ret = thread_mutex_unlock(&mutex);
		if (ret != 0)
			peixt("thread_mutex_unlock");
	}

	printf("program finish, will sleep 5s exit.\n");
	thread_sleep(5000);

	return 0;
}
