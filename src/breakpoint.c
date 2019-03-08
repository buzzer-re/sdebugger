#include "breakpoint.h"



/**
	Adds a breakpoint in a specific address
	
	@param: pid - Process ID of the target
	@param: addr - Address to add the software breakpoint
**/
uint64_t add_breakpoint(pid_t pid, uint64_t addr)
{
	uint64_t int3 = 0xCC;
	uint64_t code_at_addr = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	uint64_t code_break = (code_at_addr & ~0xFF) | int3;
	
	ptrace(PTRACE_POKEDATA, pid, addr, code_break);

	return code_at_addr;
}



/**
	Remove a breakpoint of a address
	@param: pid - Process ID of the target
	@param: addr - Address to add the software breakpoint
	@param: old_code - Old code on that address
**/
void remove_breakpoint(pid_t pid, uint64_t addr, uint64_t old_code)
{
	ptrace(PTRACE_POKEDATA, pid, addr, old_code);	
}


/**
	Step over a breakpoint, it remove the breakpoint, walk one step and continue
	@param: pid Process ID of the target
	@param: struct trap_st - Trap structure that contain the breakpoint information
**/
void step_over_breakpoint(pid_t pid, struct trap_st* trap)
{
	set_reg(pid, "rip", trap->trap_addr);
	printf("Setting register!\n");
	remove_breakpoint(pid, trap->trap_addr, trap->data_trap);
	single_step_proc(pid, WAIT);
	add_breakpoint(pid, trap->trap_addr);
}

