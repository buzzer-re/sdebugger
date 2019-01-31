#pragma once


#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Hash search table
#include <search.h>

// Sys lib
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

// Vendor lib
#include <linenoise/linenoise.h>



#include "macros.h"
#include "convert.h"
#include "breakpoint.h"

//struct reg {
//	uint64 rbp;
//	uint64 rdi;
//
//};


typedef struct dbg {
	
	pid_t target_pid;
	const char* target_name;	
	uint32_t target_status;
	
	uint8_t run : 1;	
	uint8_t target_runing: 1;
	
	uint8_t reach_breakpoint: 1;
	char* break_address;
} debugger;


void start_dbg(debugger*);
void free_dbg(debugger* );

void trace_target(debugger*);
void continue_exec(debugger*);
void enable_breakpoint(debugger*);
void input(char**, debugger*);
