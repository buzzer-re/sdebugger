#pragma once

#include <search.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>



uint64_t add_breakpoint(pid_t pid, uint64_t addr);
void remove_breakpoint(pid_t pid, uint64_t addr, uint64_t old_code);

