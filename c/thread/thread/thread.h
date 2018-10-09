#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef union
{
	unsigned int windows;
	void * linux;
}routine_t;


// os
#ifndef WIN32
	#define __WINDOWS__
	#define OS "windows"
#else
	#define __LINUX__
	#define OS "linux"
#endif


// include
#ifdef __WINDOWS__
	#include <process.h>
	#include <Windows.h>
#else

#endif

// type define
#ifdef __WINDOWS__
	typedef unsigned int routine_ret_type;
	#define thread_func_call __stdcall
	typedef HANDLE thread_t;
	typedef void*(*thread_routine)(void *);
#else
	typedef void * thread_ret_type;
#define thread_func_call
	//typedef pthread_t thread_t;
#endif



int thread_create(thread_t *thread, void *(*start_routine) (void *), void *arg, int stack_size);

#ifdef __cplusplus
}
#endif

#endif // !_THREAD_H_
