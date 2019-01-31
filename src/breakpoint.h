#pragma once

#include <search.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <stdint.h>
#include <stdlib.h>



uint32_t add_breakpoint(pid_t pid, uint32_t addr);
void remove_breakpoint(pid_t pid, uint32_t addr, uint32_t old_code);

