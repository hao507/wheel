#include "worker.h"

#define VERSION 20181111

typedef struct worker_t {

	worker_io report;
	worker_io stream;
	worker_io callback;
}worker_t;



// on success then return worker_t object pointer, on error return NULL(0).
worker_t* worker_create(int type)
{

}

// on success return 0, on error return -1.
int worker_config(char *buf, size_t len)
{

}

int worker_version()
{
	return VERSION;
}

int worker_run(worker_t *w, worker_io io)
{

}

// on success return 0, on error return -1.
int worker_destroy(worker_t *w)
{

}