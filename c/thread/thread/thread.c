/*
* Module: thread.c
*
* Purpose:
*	Implement cross-platform lightweight threading library
*
* --------------------------------------------------------------------------
*
*      threads - POSIX Threads Library for Windows/Linux
*      Copyright(C) 1996 Zhangzhongyuan
*      Contact Email: zzyservers@163.com
*
*      This library is free software; you can redistribute it and/or
*      modify it under the terms of the GNU Lesser General Public
*      License as published by the Free Software Foundation; either
*      version 2 of the License, or (at your option) any later version.
*
*      This library is distributed in the hope that it will be useful,
*      but WITHOUT ANY WARRANTY; without even the implied warranty of
*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*      Lesser General Public License for more details.
*
*      You should have received a copy of the GNU Lesser General Public
*      License along with this library in the file COPYING.LIB;
*      if not, write to the Free Software Foundation, Inc.,
*      59 Temple Place - Suite 330, Boston, MA 02111-1307, CHINA
*/

#include "thread.h"
#include "stdio.h"

thread_t thread_self(void)
{
#ifdef __WINDOWS__
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

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

int thread_equal(thread_t t1, thread_t t2)
{
#ifdef __WINDOWS__
	return t1 != t2;
#else
	return pthread_equal(t1, t2);
#endif
}

int thread_create(thread_t *thread, void *(*start_routine) (void *), void *arg, int stack_size)
{
	int ret = 0;

#ifdef __WINDOWS__
	HANDLE handle = (HANDLE)_beginthreadex(NULL, stack_size, (_beginthreadex_proc_type)start_routine, arg, 0, thread);
	if (handle == NULL)
		ret = -1;
	else
		if (CloseHandle(handle) == FALSE)
			ret = -1;
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, stack_size);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(thread, &attr, start_routine, arg);
#endif

	return ret;
}

int thread_detach(thread_t thread)
{
	int ret = 0;

#ifdef __WINDOWS__
	//if (CloseHandle(handle) == FALSE)
		//ret = -1;
#else
	ret = pthread_detach(thread);
#endif

	return ret;
}

int thread_join(thread_t thread, void ** retval)
{
	int ret = 0;

#ifdef __WINDOWS__
	HANDLE handle = OpenThread(THREAD_ALL_ACCESS, FALSE, thread);
	if (handle == NULL)
		ret = -1;
	else
	{
		if (WaitForSingleObject(handle, INFINITE) == WAIT_FAILED)
			ret = -1;
		else
			if (retval != NULL)
				if (GetExitCodeThread(handle, (unsigned int*)retval) == FALSE)
					ret = -1;

		if (CloseHandle(handle) == FALSE)
			ret = -1;
	}
#else
	ret = pthread_join(thread, retval);
#endif

	return ret;
}

int thread_cancel(thread_t thread)
{
	int ret = 0;

#ifdef __WINDOWS__
	HANDLE handle = OpenThread(THREAD_ALL_ACCESS, FALSE, thread);
	if (handle == NULL)
		ret = -1;
	else
	{
		if (TerminateThread(handle, 0) == FALSE)
			ret = -1;
		if (CloseHandle(handle) == FALSE)
			ret = -1;
	}
#else
	ret = pthread_cancel(thread);
#endif

	return ret;
}

void thread_exit(void * retval)
{
#ifdef __WINDOWS__
	_endthreadex((unsigned int)retval);
#else
	pthread_exit(retval);
#endif
}

int thread_mutex_init(thread_mutex_t * mutex)
{
	int ret = 0;

#ifdef __WINDOWS__
	if ((*mutex = CreateMutex(NULL, FALSE, NULL)) == NULL)
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
	if (WaitForSingleObject(*mutex, INFINITE) == WAIT_FAILED)
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
	if (WaitForSingleObject(*mutex, 0) == WAIT_TIMEOUT)
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
	if (ReleaseMutex(*mutex) == FALSE)
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
	if (CloseHandle(mutex) == FALSE)
		ret = -1;
#else
	ret = pthread_mutex_destroy(mutex);
#endif

	return ret;
}