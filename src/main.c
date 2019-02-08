#include <stdio.h>

#include <linenoise/linenoise.h>

#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include "debugger.h"
#include "macros.h"


int file_exists(const char* filename);

int main(int argc, char** argv) 
{

	if ( argc < 2 ) {
		fprintf(stderr, "Usage: %s <debug_target>\n", argv[0]);
		return 1;
	}

	const char* target_name = argv[1];
		
	if (!file_exists(target_name))  {
		FATAL("Cant find executable!");
		return 1;
	}	
	

		
	debugger dbg;
	dbg.target_name = target_name;
	

	start_dbg(&dbg);	

	return 0;
}


int file_exists(const char* filename) 
{
	return access(filename, F_OK) != -1;
}

