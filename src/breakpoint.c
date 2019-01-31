#include "breakpoint.h"


uint32_t add_breakpoint(pid_t pid, uint32_t addr)
{
	uint32_t int3 = 0xCC;
	uint32_t code_at_addr = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	
	uint32_t code_break = (code_at_addr & ~0xFF) | int3;

	ptrace(PTRACE_POKEDATA, pid, addr, code_break);

	return code_at_addr;
}


void remove_breakpoint(pid_t pid, uint32_t addr, uint32_t old_code)
{
	printf("Puting 0x%x back to 0x%x\n", old_code, addr)
	ptrace(PTRACE_POKEDATA, pid, addr, old_code);	
}
