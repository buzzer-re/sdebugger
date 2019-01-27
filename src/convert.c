#include "convert.h"


uint64_t str_to_hex(const char* str)
{
	return (uint64_t) strtol(str, NULL, 16);
}
