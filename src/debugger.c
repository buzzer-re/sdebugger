#include "debugger.h"



void start_dbg(debugger* dbg) 
{
	

	ENTRY handles[] = {
		// Flow control
		{"continue", &continue_exec},
		{"cont", &continue_exec},
		{"c", &continue_exec},
		{"exit", &free_dbg},
		{"quit", &free_dbg},

		// Program control
		{"break", &add_breakpoint},
		{"b", &add_breakpoint},
		{"breakpoint", &add_breakpoint}

	};
	
	uint entries_size = sizeof(handles)/sizeof(ENTRY);

	hcreate(entries_size);
	ssize_t i;
		
	for (i = 0; i < entries_size; i++) 
		hsearch(handles[i], ENTER);	
	


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
	while(dbg->run) {
		in = linenoise("debugger> ");
		input(&in, dbg);
		
		linenoiseHistoryAdd(in);
		linenoiseFree(in);
	}	

}


void continue_exec(debugger* dbg)
{

	ptrace(PTRACE_CONT, dbg->target_pid, NULL, NULL);
	fflush(stdin);
}


void add_breakpoint(debugger* dbg) 
{
	char* b_address_char = strtok(NULL, " ");
	if (!b_address_char) {
		INFO_WARN("No address supplied!");
		return ;
	}
	
	uint32_t b_address = str_to_hex(b_address_char);
	uint32_t b_opcode = 0xcc; // int 3

	uint32_t code_at_addr = ptrace(PTRACE_PEEKDATA, dbg->target_pid, b_address, NULL);
	
	// Add int 3 opcode on the bottom byte
	uint32_t break_code = (code_at_addr & ~0xff) | b_opcode;
	// Place new code on the address
	ptrace(PTRACE_POKEDATA, dbg->target_pid, b_address, break_code);		
	
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
