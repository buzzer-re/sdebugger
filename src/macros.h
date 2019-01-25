#pragma once

#include <stdio.h>
#include <unistd.h>

#define FATAL(msg)  \
	fprintf(stderr, "ERR: %s\n", msg); \
	return 1;



