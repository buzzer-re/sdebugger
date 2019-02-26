#include "proclib.h"


uint64_t read_mem(pid_t pid, uint64_t addr)
{
	uint64_t code_at = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
	
	return code_at;
}

uint64_t write_mem(pid_t pid, uint64_t addr, uint64_t data)
{
	return ptrace(PTRACE_POKEDATA, pid, addr, data);
}


void single_step_proc(pid_t pid, int wait)
{
	ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
	if(wait) waitpid(pid, 0, 0);
}


void continue_proc(pid_t pid, int wait)
{
	ptrace(PTRACE_CONT, pid, NULL, NULL);
	if (wait) waitpid(pid, 0, 0);
}
