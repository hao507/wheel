#include "thread.h"

int thread_sleep(unsigned int msec)
{
	int ret = 0;
#ifdef __WINDOWS__
	Sleep(msec);
#else
	ret = usleep(msec * 1000);
#endif
	return ret;
}

int thread_create(thread_t *thread, void *(*start_routine) (void *), void *arg, int stack_size)
{
	int ret = 0;

#ifdef __WINDOWS__
	*thread = (thread_t)_beginthreadex(0, stack_size, (_beginthreadex_proc_type)start_routine, arg, 0, 0);
	if (*thread == NULL)
		ret = -1;
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, stack_size);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(thread, &attr, start_routine, arg);
#endif

	return ret;
}

int thread_detach(thread_t thread)
{
#ifdef __WINDOWS__
	return 0;
#else
	return pthread_detach(thread);
#endif
}

int thread_join(thread_t thread, void ** retval)
{
	int ret = 0;
#ifdef __WINDOWS__
	ret = WaitForSingleObject(thread, INFINITE);
	if (ret == 0 || ret == WAIT_FAILED)
	{
		ret = 0;
		if (GetExitCodeThread(thread, (unsigned int*)retval) == 0)
			ret = -1;
	}

#else
	ret = pthread_detach(thread);
#endif

	return ret;
}

int thread_cancel(thread_t thread)
{
	int ret = 0;

#ifdef __WINDOWS__
	if (TerminateThread(thread, 0) == 0)
		ret = -1;
#else
	ret = pthread_cancel(thread);
#endif

	return ret;
}

void thread_exit(void * retval)
{
#ifdef __WINDOWS__
	//ExitThread((unsigned int)retval);
	_endthreadex((unsigned int)retval);
#else
	pthread_exit(retval);
#endif
}

int thread_mutex_init(thread_mutex_t * mutex)
{
	int ret = 0;
#ifdef __WINDOWS__
	//typedef CRITICAL_SECTION thread_mutex_t;
	//InitializeCriticalSection(mutex);
	if ((mutex = CreateMutex(NULL, FALSE, NULL)) == NULL)
		ret = -1;
#else
	ret = pthread_mutex_init(mutex, NULL);
#endif
	return ret;
}

int thread_mutex_lock(thread_mutex_t * mutex)
{
	int ret = 0;
#ifdef __WINDOWS__
	if (WaitForSingleObject(mutex, INFINITE) == 0)
		ret = -1;
#else
	ret = pthread_mutex_lock(mutex);
#endif
	return ret;
}

int thread_mutex_trylock(thread_mutex_t * mutex)
{
	int ret = 0;
#ifdef __WINDOWS__
	if (WaitForSingleObject(mutex, 0) == 0)
		ret = -1;
#else
	ret = pthread_mutex_trylock(mutex);
#endif
	return ret;
}

int thread_mutex_unlock(thread_mutex_t * mutex)
{
	int ret = 0;
#ifdef __WINDOWS__
	if (ReleaseMutex(mutex) == 0)
		ret = -1;
#else
	ret = pthread_mutex_unlock(mutex);
#endif
	return ret;
}

int thread_mutex_destroy(thread_mutex_t * mutex)
{
	int ret = 0;
#ifdef __WINDOWS__
	if (CloseHandle(mutex) == 0)
		ret = -1;
#else
	ret = pthread_mutex_destroy(mutex);
#endif
	return ret;
}