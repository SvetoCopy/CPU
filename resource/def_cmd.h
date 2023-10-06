#define StackPop(a, b) if (StackPop(a, b) == -1) { fprintf(loggerCPU.file, "Error in %d command", i + 1); return -1;}

DEF_CMD(PUSH, 1, 
	StackPush(&(cpu.stack), command.value);
	)

DEF_CMD(DIV, 2, 
	StackPop(&(cpu.stack), &b); 
	StackPop(&(cpu.stack), &a); 
	StackPush(&(cpu.stack), a / b);
	)

DEF_CMD(SUB, 3, 
	StackPop(&(cpu.stack), &b); 
	StackPop(&(cpu.stack), &a); 
	StackPush(&(cpu.stack), a - b);
	)

DEF_CMD(OUT, 4, 
	StackPop(&(cpu.stack), &a); 
	fprintf(out, "OUT = %d\n", a);
	)

DEF_CMD(HLT, -1, 
	return 0;
	)

#undef StackPop
// DEF_CMD(COS, 10, ...)