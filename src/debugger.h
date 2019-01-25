#pragma once


#include <stdio.h>

// Hash search table
#include <search.h>

// Sys lib
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

// Vendor lib
#include <linenoise/linenoise.h>

#include <string.h>

typedef struct dbg {	
	pid_t target_pid;
	const char* target_name;
	
	int target_status;
	
	int run;
	
} debugger;



void start_dbg(debugger*);
void trace_target(debugger*);
void continue_exec(debugger*);
void input(char**, debugger*);





