#pragma once

#include <stdio.h>
#include <unistd.h>

#define FATAL(msg)  \
	fprintf(stderr, "ERR: %s\n", msg); 

#define LOG_INFO(msg)\
	fprintf(stdout, "INFO: %s\n", msg);

#define LOG(msg)\
	fprintf(stdout, "%s\n", msg);

#define INFO_WARN(msg)\
	fprintf(stderr, "Warning: %s\n", msg);


#define uint64 unsigned int


