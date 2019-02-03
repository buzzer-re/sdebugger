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
#include <sys/user.h>
// Vendor lib
#include <linenoise/linenoise.h>



#include "macros.h"
#include "convert.h"
#include "breakpoint.h"
#include "register.h"

//struct reg {
//	uint64 rbp;
//	uint64 rdi;
//
//};


typedef struct dbg {
	
	pid_t target_pid;
	struct user_regs_struct regs;
	const char* target_name;	
	int target_status;
	
	uint8_t run : 1;	
	uint8_t target_runing: 1;
	uint8_t target_started: 1;	
	uint8_t reach_breakpoint: 1;
	char* break_address;
} debugger;


void start_dbg(debugger*);
void free_dbg(debugger* );

void start_target(debugger*);
void trace_target(debugger*);
void continue_exec(debugger*);
void single_step(debugger*);
void enable_breakpoint(debugger*);
void input(char**, debugger*);


// Features

void dump_registers_wr(debugger* dbg);
