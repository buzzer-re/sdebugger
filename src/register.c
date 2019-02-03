#include "register.h"


void dump_registers(pid_t pid) 
{
//44-  __extension__ unsigned long long int r15;
//45-  __extension__ unsigned long long int r14;
//46-  __extension__ unsigned long long int r13;
//47-  __extension__ unsigned long long int r12;
//48-  __extension__ unsigned long long int rbp;
//49-  __extension__ unsigned long long int rbx;
//50-  __extension__ unsigned long long int r11;
//51-  __extension__ unsigned long long int r10;
//52-  __extension__ unsigned long long int r9;
//53-  __extension__ unsigned long long int r8;
//54-  __extension__ unsigned long long int rax;
//55-  __extension__ unsigned long long int rcx;
//56-  __extension__ unsigned long long int rdx;
//57-  __extension__ unsigned long long int rsi;
//58-  __extension__ unsigned long long int rdi;
//59-  __extension__ unsigned long long int orig_rax;
//60-  __extension__ unsigned long long int rip;
//61-  __extension__ unsigned long long int cs;
//62-  __extension__ unsigned long long int eflags;
	
	
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
