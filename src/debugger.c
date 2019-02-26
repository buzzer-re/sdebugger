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
	breakpoint_limits = 100;
	hcreate_r(entries_size, &dispatch_table);
	hcreate_r(breakpoint_limits, &breakpoint_table);
	ssize_t i;
	
	ENTRY* tmp; // Why do that if i just wanna enter the value?...
	for (i = 0; i < entries_size; ++i)
		hsearch_r(handles[i], ENTER, &tmp, &dispatch_table);

	dbg->target_runing = 0;
	dbg->reach_breakpoint = 0;
	dbg->target_started = 0;

	
	init_reg_table();
	start_target(dbg);
	trace_target(dbg);
}


void free_dbg(debugger* dbg)
{
	hdestroy_r(&dispatch_table);
	hdestroy_r(&breakpoint_table);
	destroy_registers();	
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
		char in;
		printf("Child already started in %d, wanna restart? [y/N]: ", dbg->target_pid);
		in = getchar();
		CLEAN_STDIN();
		if (in != 'y' && in != 'Y') return ;	
		kill(dbg->target_pid, SIGKILL);
	}	

	dbg->target_pid = fork();
	if (!dbg->target_pid) {
		ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
		execl(dbg->target_name, dbg->target_name, NULL);
		exit(1); //sanity check
	} else {
		dbg->target_started = TARGET_STARTED;
		printf("Child pid ready at %d\n", dbg->target_pid);
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
			hdestroy_r(&breakpoint_table);
			hcreate_r(breakpoint_limits, &breakpoint_table);		
			printf("Child %d exited!\n", dbg->target_pid);
		}
		else {
			signal = WSTOPSIG(dbg->target_status);
			
			dbg->trap.trap_addr = get_pc(dbg->target_pid);
			dbg->trap.data_trap = read_mem(dbg->target_pid, dbg->trap.trap_addr);	

			switch(signal) {
				case SIGSEGV:
					fprintf(stderr, "%s Dump: 0x%x IP: 0x%x\n", strsignal(signal), dbg->trap.data_trap, dbg->trap.trap_addr - 1);
					break;
				case SIGTRAP:
					fprintf(stdout, "Breakpoint reached at 0x%x\n", dbg->trap.trap_addr);
					dbg->target_runing = TARGET_STOPED;	
					break;
			}
		}

	}

}


void continue_exec(debugger* dbg)
{

	ASSERT_TARGET_RUNING(dbg->target_started);

	if (dbg->reach_breakpoint)
	{
			
		char* rip_addr = hex_to_str(dbg->trap.trap_addr);
		ENTRY search_break = {rip_addr};
		ENTRY* break_table; 
		hsearch_r(search_break, FIND, &break_table, &breakpoint_table);
		free(rip_addr);

		if (break_table != NULL) {
			step_over_breakpoint(dbg->target_pid, &dbg->trap);
			LOG("Continuing exec!");
		}
		dbg->reach_breakpoint = 0;
	}

	dbg->target_runing = 1;

	continue_proc(dbg->target_pid, NOWAIT);

	waitpid(dbg->target_pid, &dbg->target_status, 0);
}


void single_step(debugger* dbg)
{
	ASSERT_TARGET_RUNING(dbg->target_started);
	
	single_step_proc(dbg->target_pid, NOWAIT);
	waitpid(dbg->target_pid, &dbg->target_status, 0);
	uint64_t ip = get_pc(dbg->target_pid) - 1;

	printf("Step to 0x%x\n", ip);
}

void enable_breakpoint(debugger* dbg)
{
	ASSERT_TARGET_RUNING(dbg->target_started);
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
	ENTRY* h_table_res;
	hsearch_r(breakpoint, FIND, &h_table_res, &breakpoint_table);

	if (h_table_res != NULL) {
		INFO_WARN("This address already has a breakpoint!");
		printf("With code: 0x%x\n", (uint64_t) h_table_res->data);
		return ;
	}

	breakpoint.data = (void*) add_breakpoint(dbg->target_pid, b_address);

	hsearch_r(breakpoint, ENTER, &h_table_res, &breakpoint_table);

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
	ENTRY* res;

	hsearch_r(handle, FIND, &res, &dispatch_table);
	if (res != NULL)
		( ( void(*) (void *) ) res->data) (dbg);

	else
		INFO_WARN("Invalid command!");
}





//wrapper
void dump_registers_wr(debugger* dbg)
{
	char* target_reg = strtok(NULL, " ");
	if (!target_reg)
		dump_registers(dbg->target_pid);

	else { 
		dump_register(dbg->target_pid, target_reg);
	}
}
