#ifndef __WORKER_H__
#define __WORKER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>

#define SOURCE 0
#define PROCESS 1
#define TARGET 2

typedef struct worker_t {
	int;
}worker_t;

typedef struct worker_io {
	// pointer assigned by the user himself.
	void *user;

	// data sources.
	// return value: on success, the  number of bytes read is returned (zero indicates end of file)
	// on error, the number of -1 is returned.
	int(*in)(char *buf, size_t len, void *user);

	// data processor.
	// return value: on success, the number of bytes written is returned (zero indicates nothing was  written).
	// on error, the number of -1 is returned.
	int(*process)(const char *in, size_t len_in, char *out, size_t len_out, void *user);

	// data output.
	// return value: on success, the number of bytes out is returned(zero indicates end of file)
	// on error, the number of -1 is returned.
	int(*out)(const char *data, size_t len, void *user);

	// reset environment.
	// return value: on success, the number of zero is returned.
	// on error, the number of -1 is returned.
	int(*reset)(void *user);
}worker_io;


// on success then return worker_t object pointer, on error return NULL(0).
worker_t* worker_create(int type);

// on success return 0, on error return -1.
int worker_config(char *buf, size_t len);

int worker_version();

int worker_run(worker_t *w, worker_io io);

// on success return 0, on error return -1.
int worker_destroy(worker_t *w);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !__WORKER_H__
