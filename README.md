<h1>CPU, Assembler, Disassembler by Ruzal</h1>
<h2>Commands Example</h2>

| Command | Description                                        | Arg Types     |
|---------|----------------------------------------------------|---------------|
| PUSH    | Pushes a value onto the stack                      | IMM, REG, RAM |
| POP     | Popes a value from the stack                       | IMM, REG, RAM |
| DIV     | Take two values from the stack divides             |       -       |
| SUB     | Take two values from the stack subtracts           |       -       |
| OUT     | Print a value from the stack head                  |       -       |
| IN      | Pushes a value to the stack                        |       -       |
| JMP     | Jump to the any part of code                       | IMM, REG, RAM |
| CALL    | Pop a value from the stack and  jump to this value |       -       |

Other commands listed in def_cmd.h

<h2>Register Agreement</h2>

| RAX | Return register |
|-----|-----------------|
| RBX | first arg       |
| RCX | second arg      |
| RDX | third arg       |
| R8  | calee saved     |
| R9  | calee saved     |
| R10 | calee saved     |

<h2>Video RAM</h2>
<p>Initially, RAM stores double. But for Video RAM these doubles are perceived as ASKII codes</p>
<h3>Example</h3>

```
RAM[0] = 42.000
PrintVideoRAM():
  char c = 42; 
  pritnf("%c", c); 
```
