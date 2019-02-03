# sdebugger
A simple debugger for Linux on development



## Building

You will need [cmake](https://cmake.org) in order to build properly:
```shell
mkdir -p build/
cd build/
cmake ..
make
```


### Using

For now still veeery simple, you can send your own binary to it and continue or add breakpoint. that's it for now :D 

```
./main <binary>
debugger> r // Spawn child process (run)
debugger> b <addr> // Add a breakpoint to a given addr (break, breakpoint, b)
debugger> dumpr // Dump registers, put a breakpoint before
debugger> s // Make a single step (step,s)
debugger> cont // continue where its stopped or start process (c, cont, continue)
```

#### TODO


 | List  | Description|
| ------------- | -------------|
| Multiple Breakpoint | Capability for create multiple breakpoint |
| Show registers  |  Capability to show registers when debugging |
| Modify registers | Capability to modify a register value |
| Dissasembly | Capability to disassembly a function or a range of address |
| Patch | Capability to patch some opcode on the binary |
