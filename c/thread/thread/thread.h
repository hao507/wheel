#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

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
	#include <pthread.h>
#endif

// type define
#ifdef __WINDOWS__
	typedef HANDLE thread_t;
	typedef HANDLE thread_mutex_t;
#else
	typedef pthread_t thread_t;
	typedef pthread_mutex_t thread_mutex_t;
#endif

// thread
int thread_sleep(unsigned int msec);
int thread_create(thread_t *thread, void *(*start_routine) (void *), void *arg, int stack_size);
int thread_detach(thread_t thread);
int thread_join(thread_t thread, void **retval);
int thread_cancel(thread_t thread);
void thread_exit(void *retval);

// mutex
int thread_mutex_init(thread_mutex_t *mutex);
int thread_mutex_lock(thread_mutex_t *mutex);
int thread_mutex_trylock(thread_mutex_t *mutex);
int thread_mutex_unlock(thread_mutex_t *mutex);
int thread_mutex_destroy(thread_mutex_t *mutex);

// cond


/*

thread_attr_init�����������߳��Ƿ���������
thread_attr_destroy()
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
thread_kill���������̷߳���kill�ź�


thread_cond_init��������ʼ����������
thread_cond_signal�����������źŻ��ѽ���
thread_cond_wait�������ȴ����������������¼�����
thread_cond_destroy(&g_cond);

*/

#ifdef __cplusplus
}
#endif

#endif // !_THREAD_H_
