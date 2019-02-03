#include "debugger.h"

// Create a table only for breakpoints address

void start_dbg(debugger* dbg) 
{
	

	ENTRY handles[] = {
		// Flow control
		{"run", &start_target},
		{"r", &start_target},
		{"continue", &continue_exec},
		{"cont", &continue_exec},
		{"c", &continue_exec},
		{"exit", &free_dbg},
		{"quit", &free_dbg},
		{"q", &free_dbg},

		// Program control
		{"break", &enable_breakpoint},
		{"b", &enable_breakpoint},
		{"breakpoint", &add_breakpoint},
		{"step", &single_step},
		{"s", &single_step},
		//Features
		{"dumpr", &dump_registers_wr},

	};
	
	uint64_t entries_size = sizeof(handles)/sizeof(ENTRY);
	uint64_t breakpoints_limit = 100;
	
	hcreate(entries_size + breakpoints_limit);
	ssize_t i;
		
	for (i = 0; i < entries_size; i++) 
		hsearch(handles[i], ENTER);	
	
	dbg->target_runing = 0;
	dbg->reach_breakpoint = 0;
	
	trace_target(dbg);	
}


void free_dbg(debugger* dbg)
{
	hdestroy();
	dbg->run = 0;
	if (dbg->target_started) { 
		kill(dbg->target_pid, SIGKILL);
		printf("Killed child with pid: %d\nExiting...\n", dbg->target_pid);
	}
	else
		ptrace(PTRACE_DETACH, dbg->target_pid, NULL, NULL);
}

void start_target(debugger* dbg)
{
	if (dbg->target_started) {
		INFO_WARN("Target already beeing debugged!");
		return ;
	}
	dbg->target_pid = fork();
	if (!dbg->target_pid) {
		ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
		execl(dbg->target_name, dbg->target_name, NULL);
	} else {
		dbg->target_started = 1;
		printf("Child pid started at %d\n", dbg->target_pid);
	}
}

void trace_target(debugger* dbg) 
{	
	char* in;
	dbg->run = 1;
	uint8_t signal;
	while(dbg->run) {
		if (!dbg->target_runing) {
			in = linenoise("debugger> ");
			
			linenoiseHistoryAdd(in);
			input(&in, dbg);


			linenoiseFree(in);
		} 
		else if (WIFEXITED(dbg->target_status)) {
			dbg->target_started = 0;
			dbg->target_runing = 0;
			printf("Child %d exited!\n", dbg->target_pid);

		}
		else {	
			signal = WSTOPSIG(dbg->target_status);	

			if (signal != SIGTRAP)  {
				ptrace(PTRACE_GETREGS, dbg->target_pid, NULL, &dbg->regs);
				uint64_t code = ptrace(PTRACE_PEEKDATA, dbg->target_pid, --dbg->regs.rip, NULL);
				printf("%s Code: 0x%x RIP=> 0x%x\n", strsignal(signal), code,dbg->regs.rip);

				waitpid(dbg->target_pid, &dbg->target_status, 0);			
			}
			else {
				LOG("Breakpoint reached!");
				dbg->target_runing = 0;
				dbg->reach_breakpoint = 1;
			} 
		}
					
	}	

}


void continue_exec(debugger* dbg)
{
	
	if (!dbg->target_started) {
		INFO_WARN("Target are not runing!");
		return ;
	}
	if (dbg->reach_breakpoint)
	{
		
		ptrace(PTRACE_GETREGS, dbg->target_pid, NULL, &dbg->regs);
		dbg->regs.rip--;
		char* rip_addr = hex_to_str(dbg->regs.rip);
		ENTRY search_break = {rip_addr};
		ENTRY* break_table = hsearch(search_break, FIND);
		free(rip_addr);
			
		if (break_table != NULL) {	
			remove_breakpoint(dbg->target_pid, dbg->regs.rip, (uint64_t) break_table->data);	
			ptrace(PTRACE_SETREGS, dbg->target_pid, NULL, &dbg->regs);
			ptrace(PTRACE_SINGLESTEP, dbg->target_pid, NULL, NULL);
			waitpid(dbg->target_pid, &dbg->target_status, 0);
			add_breakpoint(dbg->target_pid, dbg->regs.rip);	
		
			LOG("Continuing exec!");
		}
		dbg->reach_breakpoint = 0;
	}
	dbg->target_runing = 1;
	ptrace(PTRACE_CONT, dbg->target_pid, NULL, NULL);
	waitpid(dbg->target_pid, &dbg->target_status, 0);
}


void single_step(debugger* dbg)
{
	if (!dbg->target_started) {
		INFO_WARN("Target are not runing!");
		return ;
	}
	ptrace(PTRACE_SINGLESTEP, dbg->target_pid, NULL, NULL);
	waitpid(dbg->target_pid, &dbg->target_status, 0);
	uint64_t ip = get_pc(dbg->target_pid) - 1;
	printf("Step to 0x%x\n", ip);
}

void enable_breakpoint(debugger* dbg) 
{

	if (!dbg->target_started) {
		INFO_WARN("Target are not runing!(loaded) run 'r' first!");
		return ;
	}
	char* b_address_char = strtok(NULL, " ");

	if (!b_address_char) {
		INFO_WARN("No address supplied!");
		return ;
	}
		
	uint64_t b_address = str_to_hex(b_address_char);
	if (!b_address) {
		INFO_WARN("Invalid address!");
		return ;
	}
	
	ENTRY breakpoint = {b_address_char};
	if (hsearch(breakpoint, FIND) != NULL) {
		INFO_WARN("This address already has a breakpoint!");
		printf("With code: 0x%x\n", (uint64_t) hsearch(breakpoint,FIND)->data);
		return ;
	}
	
	breakpoint.data = (void*) add_breakpoint(dbg->target_pid, b_address);

	hsearch(breakpoint, ENTER);
	
	fprintf(stdout, "Breakpoint on 0x%x\n", b_address);	

}




// Process user input on linenoise by using strtok to break the string into tokens
// the first token is the user command, and the others tokens is pulled out by the requested command function that
// are called on the dispatch table.
//
// If no handlers are found (invalid command), a error message appears and nothing happens
//
void input(char** input, debugger* dbg) 
{
	
	char* handler_name = strtok(*input, " ");
	ENTRY handle = {handler_name};
	
	if (hsearch(handle, FIND) != NULL)	
		( ( void(*) (void *) ) hsearch(handle, FIND)->data) (dbg);
	
	else
		INFO_WARN("Invalid command!");
		//system(handler_name);
}




// Other features



//wrapper
void dump_registers_wr(debugger* dbg)
{
	dump_registers(dbg->target_pid);
}
