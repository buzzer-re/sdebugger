#include "breakpoint.h"


uint64_t add_breakpoint(pid_t pid, uint64_t addr)
{
	uint64_t int3 = 0xCC;
	uint64_t code_at_addr = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	printf("Code at 0x%x => 0x%x\n", addr, code_at_addr);	
	uint64_t code_break = (code_at_addr & ~0xFF) | int3;
	code_at_addr &= 0xff; //get the bottom byte(last)
	ptrace(PTRACE_POKEDATA, pid, addr, code_break);

	return code_at_addr;
}


void remove_breakpoint(pid_t pid, uint64_t addr, uint64_t old_code)
{
	printf("Puting 0x%x back to 0x%x\n", old_code, addr);
	uint64_t code_at = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	code_at = (code_at & ~0xFF) | old_code;
	printf("New code: 0x%x\n", code_at);
	ptrace(PTRACE_POKEDATA, pid, addr, code_at);	
}
