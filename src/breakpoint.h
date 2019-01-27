#pragma once

#include <search.h>

struct breakpoint {

	uint32_t** addr_code; // addr_code[address][code]
	
};



void add_breakpoint(uint32_t addr, pid_t pid, breakpoint* bp);



void add_breakpoint()
{
	ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	
	bp.
}
