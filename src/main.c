#include <stdio.h>

#include <linenoise/linenoise.h>

#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>


#include "debugger.h"
#include "macros.h"


int file_exists(const char* filename);

int main(int argc, char** argv) 
{

	/*if ( argc < 2 ) {
		fprintf(stderr, "Usage: %s <debug_target>\n", argv[0]);
		return 1;
	}*/

	const char* target_name = argc > 2 ? argv[1] : "/usr/bin/ls";
		
	if (!file_exists(target_name))  {
		FATAL("Cant find executable!");
	}	
	
	// Split process (fork)
	
	pid_t child_pid = fork();
	
	debugger dbg = {child_pid, target_name, 0};

	if (child_pid == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl(target_name, target_name, NULL);
	}	else{
		start_dbg(&dbg);
	}

	return 0;
}


int file_exists(const char* filename) 
{
	return access(filename, F_OK) != -1;
}
