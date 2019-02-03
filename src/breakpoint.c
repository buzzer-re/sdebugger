#include "breakpoint.h"


uint64_t add_breakpoint(pid_t pid, uint64_t addr)
{
	uint64_t int3 = 0xCC;
	uint64_t code_at_addr = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	uint64_t code_break = (code_at_addr & ~0xFF) | int3;
	
	ptrace(PTRACE_POKEDATA, pid, addr, code_break);

	return code_at_addr;
}


void remove_breakpoint(pid_t pid, uint64_t addr, uint64_t old_code)
{
	uint64_t code_at = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	ptrace(PTRACE_POKEDATA, pid, addr, code_at);	
}
