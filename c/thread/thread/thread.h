/*
* Module: thread.h
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

#ifndef __THREAD_H__
#define __THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

// os
#if (defined _WIN32) || (defined _WIN64)
	#define __WINDOWS__
	#define __OS__ "windows"
#else
	#define __LINUX__
	#define __OS__ "linux"
#endif

// include
#ifdef __WINDOWS__
	#include <Windows.h>
	#include <process.h>
#else
	#include <unistd.h>
	#include <pthread.h>
#endif

// type define
#ifdef __WINDOWS__
	//typedef struct {
	//	DWORD		pid;
	//	HANDLE		handle;
	//}thread_t;
	typedef DWORD			thread_t;
	typedef HANDLE			thread_mutex_t;
	typedef HANDLE			thread_cond_t;
#else
	typedef pthread_t		thread_t;
	typedef pthread_mutex_t	thread_mutex_t;
	typedef pthread_cond_t	thread_cond_t;
#endif

// thread
extern thread_t thread_self(void);
extern int thread_sleep(unsigned int msec);
extern int thread_equal(thread_t t1, thread_t t2);

extern int thread_create(thread_t *thread, void *(*start_routine) (void *), void *arg, int stack_size);
extern int thread_detach(thread_t thread);
extern int thread_join(thread_t thread, void **retval);
extern int thread_cancel(thread_t thread);
extern void thread_exit(void *retval);

// mutex
extern int thread_mutex_init(thread_mutex_t *mutex);
extern int thread_mutex_lock(thread_mutex_t *mutex);
extern int thread_mutex_trylock(thread_mutex_t *mutex);
extern int thread_mutex_unlock(thread_mutex_t *mutex);
extern int thread_mutex_destroy(thread_mutex_t *mutex);

// cond



/*
int pthread_cond_init(thread_cond_t *cond, thread_condattr_t *cond_attr);
int pthread_cond_signal(thread_cond_t *cond);
int pthread_cond_broadcast(thread_cond_t *cond);
int pthread_cond_wait(thread_cond_t *cond, thread_mutex_t *mutex);
int pthread_cond_timedwait(thread_cond_t  *cond, thread_mutex_t  *mutex, const struct timespec *abstime);
int pthread_cond_destroy(thread_cond_t *cond);

*/

/*

thread_attr_init（）：设置线程是否脱离属性
thread_attr_destroy()
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);

*/

#ifdef __cplusplus
}
#endif

#endif // !__THREAD_H__
