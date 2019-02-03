#include "convert.h"


uint64_t str_to_hex(const char* str)
{

	return (uint64_t) strtol(str, NULL, 16);
}


char* hex_to_str(uint64_t hex)
{
	char* hex_str = malloc(sizeof(char)*10); //("0xFFFFFFFF"));
	sprintf(hex_str, "0x%x", hex);

	return hex_str;
}

