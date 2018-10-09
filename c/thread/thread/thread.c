#include "thread.h"

int thread_create(thread_t *thread, void *(*start_routine) (void *), void *arg, int stack_size)
{
	int ret = 0;
#ifdef __WINDOWS__
	*thread = (thread_t)_beginthreadex(0, stack_size, (_beginthreadex_proc_type)start_routine, arg, 0, 0);
	if (*thread == NULL)
	{
		ret = -1;
	}
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, stack_size);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(thread, &attr, start_routine, arg);
#endif

	return ret;
}