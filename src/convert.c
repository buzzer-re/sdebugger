#include "convert.h"


uint64_t str_to_hex(const char* str)
{

	return (uint64_t) strtol(str, NULL, 16);
}


char* hex_to_str(uint64_t hex)
{
	int hex_str_size = 11;
	char* hex_str = calloc(hex_str_size, sizeof(char)); //("0xFFFFFFFF"));
	sprintf(hex_str, "0x%x", hex);
	

	#ifdef DEBUG
	STR_BYTES(hex_str);
	#endif
	return hex_str;
}

