#pragma once

#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define __USE_GNU
#include <search.h>



struct user_regs_struct regs;

struct hsearch_data registers_mapped;


void init_reg_table();
void destroy_registers();

void dump_registers(pid_t pid);

void dump_register(pid_t pid, char* reg);

uint64_t get_pc(pid_t pid);

