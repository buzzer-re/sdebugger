#pragma once


#include <sys/types.h>
#include <sys/ptrace.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


#include "proclib.h"
#include "register.h"

struct trap_st {
	uint64_t trap_addr;
	uint64_t data_trap;
};


uint64_t add_breakpoint(pid_t pid, uint64_t addr);
void remove_breakpoint(pid_t pid, uint64_t addr, uint64_t old_code);

void step_over_breakpoint(pid_t pid, struct trap_st* trap);

