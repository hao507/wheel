#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "worker.h"

int in(char *buf, size_t len, void *user)
{
	if (buf == NULL || len <= 0)
		return -1;

	const char *msg = "this is source demo.";
	size_t length = strlen(msg);
	if (len < length)
		length = len;

	strncpy(buf, msg, length);

	return length;
}
    
int process(const char *in, size_t len_in, char *out, size_t len_out, void *user)
{
	if (in == NULL || len_in <= 0 || out == NULL || len_out <= 0)
		return -1;

	const char *msg = " into the processor.";
	size_t len_msg = strlen(msg);
	int len = 0;

	if (len_out > len_in) {
		strncpy(out, in, len_in);
		if (len_out - len_in < len_msg)
			len_msg = len_out - len_in;

		strncat(out, msg, len_msg);
		len = len_in + len_msg;
	}
	else {
		strncpy(out, in, len_out);
		len = len_out;
	}

	return len;
}
    
int out(const char *data, size_t len, void *user)
{
	if (data == NULL || len <= 0)
		return -1;

	printf("source out[%d]:", len);

	for (size_t i = 0; i < len; i++)
		putchar(data[i]);

	putchar('\n');

	return len;
}
    
int reset(void *user)
{
	return 0;
}

int test_source()
{
	char buf[1024] = { 0 };
	int len = in(buf, sizeof(buf), NULL);
	if (len < 0)
		return -1;

	char data[1024] = { 0 };
	len = process(buf, len, data, sizeof(data), NULL);
	if (len < 0)
		return -1;

	len = out(data, len, NULL);
	if (len < 0)
		return -1;

	system("pause");
	return EXIT_SUCCESS;
}

int main()
{
	worker_t *worker = worker_create(SOURCE);
	if (worker == NULL) {
		return -1;
	}

	worker_io io;
	worker_run(worker, io);


	system("pause");
	return EXIT_SUCCESS;
}

