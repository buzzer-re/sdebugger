#include "debugger.h"

// Create a table only for breakpoints address

void start_dbg(debugger* dbg) 
{
	

	ENTRY handles[] = {
		// Flow control
		{"continue", &continue_exec},
		{"cont", &continue_exec},
		{"c", &continue_exec},
		{"exit", &free_dbg},
		{"quit", &free_dbg},
		{"q", &free_dbg},

		// Program control
		{"break", &enable_breakpoint},
		{"b", &enable_breakpoint},
		{"breakpoint", &add_breakpoint}

	};
	
	uint32_t entries_size = sizeof(handles)/sizeof(ENTRY);
	uint32_t breakpoints_limit = 100;
	
	printf("Total itens: %d\n",entries_size + breakpoints_limit);
	hcreate(entries_size + breakpoints_limit);
	ssize_t i;
		
	for (i = 0; i < entries_size; i++) 
		hsearch(handles[i], ENTER);	
	
	dbg->target_runing = 0;
	dbg->reach_breakpoint = 0;
	dbg->target_pid = fork();
	
	if (!dbg->target_pid) {
		ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
		execl(dbg->target_name, dbg->target_name, NULL);
	} else 
		trace_target(dbg);
}


void free_dbg(debugger* dbg)
{
	hdestroy();
	dbg->run = 0;
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
		} else {	
			signal = WSTOPSIG(dbg->target_status);	
			if (signal != SIGTRAP) 
				waitpid(dbg->target_pid, &dbg->target_status, 0);			
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
	
	if (dbg->reach_breakpoint)
	{
		ENTRY search_break = {"0x401112"};
		ENTRY* break_table = hsearch(search_break, FIND);
			
		if (break_table != NULL) {	
			remove_breakpoint(dbg->target_pid, /*break_table->key*/0x401112, (uint32_t) break_table->data);	
			LOG("Breakpoint removed!, now decrease your program counter and walk another step for load the old code in memory!\n");
		}
		dbg->reach_breakpoint = 0;
	}
	dbg->target_runing = 1;
	ptrace(PTRACE_CONT, dbg->target_pid, NULL, NULL);
}


void enable_breakpoint(debugger* dbg) 
{
	char* b_address_char = strtok(NULL, " ");
	//const char b_address_char_cp[strlen(b_address_char)];
	//strcpy(b_address_char_cp, b_address_char);
	char* b_address_char_cp = "0x401112";

	if (!b_address_char) {
		INFO_WARN("No address supplied!");
		return ;
	}
		
	uint32_t b_address = str_to_hex(b_address_char_cp);
	if (!b_address) {
		INFO_WARN("Invalid address!");
		return ;
	}
	
	ENTRY breakpoint = {b_address_char_cp};
	if (hsearch(breakpoint, FIND) != NULL) {
		INFO_WARN("This address already has a breakpoint!");
		printf("With code: 0x%x\n", (uint32_t) hsearch(breakpoint,FIND)->data);
		return ;
	}
	
	uint32_t code_at_addr = add_breakpoint(dbg->target_pid, b_address);
	
	breakpoint.data = (void*) code_at_addr;
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
