#pragma once

#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



struct user_regs_struct regs;

void dump_registers(pid_t pid);
uint64_t get_pc(pid_t pid);

