#include "register.h"



void init_reg_table()
{

	ENTRY registers_entries[] = {
		{"rip", &regs.rip},
		{"rax", &regs.rax},
		{"rsi", &regs.rsi},
		{"rdx", &regs.rdx},
		{"rsi", &regs.rsi},
		{"rcx", &regs.rcx},
		{"rbp", &regs.rbp},
		{"rdi", &regs.rdi},
		{"cs", &regs.cs},
		{"eflags", &regs.eflags},
		{"r15", &regs.r15},
		{"r14", &regs.r14},
		{"r12", &regs.r12},
		{"r11", &regs.r11}
	};
	ssize_t i;
	uint64_t reg_table_size = sizeof(registers_entries)/sizeof(ENTRY);
	
	ENTRY* tmp;
	hcreate_r(reg_table_size, &registers_mapped);
	for(i = 0; i < reg_table_size; ++i) {
		hsearch_r(registers_entries[i], ENTER, &tmp, &registers_mapped);
	}
}


void dump_registers(pid_t pid) 
{
	
	
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	
	printf("R15: 0x%x\n", regs.r15);
	printf("R14: 0x%x\n", regs.r14);
	printf("R12: 0x%x\n", regs.r12);
	printf("R11: 0x%x\n", regs.r11);
	printf("R10: 0x%x\n", regs.r10);
	printf("RBP: 0x%x\n", regs.rbp);
	printf("RAX: 0x%x\n", regs.rax);
	printf("RCX: 0x%x\n", regs.rcx);
	printf("RDX: 0x%x\n", regs.rdx);
	printf("RSI: 0x%x\n", regs.rsi);
	printf("RDI: 0x%x\n", regs.rdi);
	printf("RIP: 0x%x\n", regs.rip);
	printf("CS:  0x%x\n", regs.cs);
	printf("EFGLAS: 0x%x\n", regs.eflags);
	
}


uint64_t get_pc(pid_t pid) 
{
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);

	return (uint64_t) regs.rip;
}


void dump_register(pid_t pid, char* reg)
{
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	ENTRY search_reg = {reg, NULL};
	ENTRY* res;
	hsearch_r(search_reg, FIND, &res, &registers_mapped);

	if (res != NULL) 
		printf("%s: 0x%x\n", reg, (uint64_t) res->data);
	else
		printf("Invalid register\n");
}



void destroy_registers() 
{
	hdestroy_r(&registers_mapped);
}
