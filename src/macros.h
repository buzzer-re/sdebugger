#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FATAL(msg)  \
	fprintf(stderr, "ERR: %s\n", msg);

#define LOG_INFO(msg)\
	fprintf(stdout, "INFO: %s\n", msg);

#define LOG(msg)\
	fprintf(stdout, "%s\n", msg);

#define INFO_WARN(msg)\
	fprintf(stderr, "Warning: %s\n", msg);

#define ASSERT_TARGET_RUNING(status)\
	if (!status) {\
		fprintf(stderr, "%s\n","Target are not runing, run r first!\n");\
		return;\
	}\

#define CLEAN_STDIN()\
	char buff;\
	while((buff = getchar()) != '\n' && buff != EOF);



#define STR_BYTES(str)\
	puts("Raw bytes: ");\
	for(ssize_t i = 0; i < strlen(str); ++i) \
		fprintf(stdout, "0x%x ", str[i]);
