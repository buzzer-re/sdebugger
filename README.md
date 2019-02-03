# sdebugger
A simple x64 debugger for Linux on development



## Building

You will need [cmake](https://cmake.org) in order to build properly:
```shell
mkdir -p build/
cd build/
cmake ..
make
```


### Using

For now still veeery simple.

```
./main <binary>
debugger> r // Spawn child process (run)
debugger> b <addr> // Add a breakpoint to a given addr (break, breakpoint, b)
debugger> dumpr // Dump registers, put a breakpoint before
debugger> s // Make a single step (step,s)
debugger> cont // continue where its stopped or start process (c, cont, continue)
```


#### Example

```
$ objdump -DM intel ../examples/hello | grep '<main>'                                                                                                                                       
289:0000000000401122 <main>:
$./sdebugger ../examples/hello                                                                                                                                                             debugger> r
Child pid started at 28536
debugger> b 0x401122
Breakpoint on 0x401122
debugger> c
Breakpoint reached!
debugger> dumpr
[Registers dump]
debugger> s
Step to 0x401125
debugger> s
Step to 0x40112c
debugger> c
Hello world
Child 28536 exited!
debugger> 
```
#### TODO


 | List  | Description|
| ------------- | -------------|
| Modify registers | Capability to modify a register value |
| Dissasembly | Capability to disassembly a function or a range of address |
| Patch | Capability to patch some opcode on the binary |
