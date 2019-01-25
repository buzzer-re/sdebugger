#include "debugger.h"


void start_dbg(debugger* dbg) 
{

	if (!hcreate(1)) {
		printf("Cant create hashmap\n");
	}
	
	ENTRY continue_handle = {"continue", &continue_exec};
	hsearch(continue_handle, ENTER);
	
	trace_target(dbg);
}



void trace_target(debugger* dbg) 
{	
	char* in;
	dbg->run = 1;
	while(dbg->run) {
		in = linenoise("debugger> ");
		input(&in, dbg);
	}	

}


void continue_exec(debugger* dbg)
{
	ptrace(PTRACE_CONT, dbg->target_pid, NULL, NULL);
	fflush(stdin);
}


/* 	It uses <search.h> to build a hashmap, that hashmap takes as key 
 *  	char* => function ptr
 *
 *  First search on that map, if that key exists, make this weird cast to the void* pointer
 *
 */

void input(char** input, debugger* dbg) 
{
	ENTRY handle = {*input};
	
	if (hsearch(handle, FIND) != NULL)	
		( ( void(*) (void *) ) hsearch(handle, FIND)->data) (dbg);

}
