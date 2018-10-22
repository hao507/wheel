#pragma once
#include "command.h"

static int ARGC = 0;
static const char** ARGV = 0;

//getopt

int cmd_init(int argc, const char* argv[])
{
	if (ARGV != 0)
		return -1;

	ARGC = argc;
	ARGV = argv;

	return 0;
}

