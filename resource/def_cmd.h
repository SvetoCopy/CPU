#define StackPop(a, b) if (StackPop(a, b) == -1) { fprintf(stderr, "Error in %d command", line_iter + 1); return -1;}

DEF_CMD(PUSH, 1,
	if (opcode.arg_type == 1) StackPush(&(spu.stack), value);
	else if (opcode.arg_type == 2) {
		spu_var = (Stack*)(&spu) + 1;
		reg_var = (double*)(spu_var) + reg_num;
		StackPush(&(spu.stack), *reg_var);
	}
	else return -1;
	)

DEF_CMD(DIV, 2, 

	StackPop(&(spu.stack), &b); 
	StackPop(&(spu.stack), &a);

	StackPush(&(spu.stack), a / b);
	)

DEF_CMD(SUB, 3, 
	StackPop(&(spu.stack), &b); 
	StackPop(&(spu.stack), &a); 
	StackPush(&(spu.stack), a - b);
	)

DEF_CMD(OUT, 4, 
	StackPop(&(spu.stack), &a);
	printf("OUT = %f\n", a);
	fprintf(out, "OUT = %f\n", a);
	)
DEF_CMD(IN, 5,
	printf(">");
	scanf_s("%d", &in_var);
	StackPush(&(spu.stack), in_var);
	)
DEF_CMD(MUL, 6,
	StackPop(&(spu.stack), &b);
	StackPop(&(spu.stack), &a);
	StackPush(&(spu.stack), a * b);
)
DEF_CMD(SIN, 7,
	StackPop(&(spu.stack), &a);
	StackPush(&(spu.stack), sinf(a));
)
DEF_CMD(COS, 8,
	StackPop(&(spu.stack), &a);
	StackPush(&(spu.stack), cosf(a));
)


DEF_CMD(HLT, 9, 
	return 0;
	)
DEF_CMD(POP, 10,
	if (opcode.arg_type == 1) {
		fprintf(stderr, "IMM in POP argument");
		return -1;
	}
	spu_var = (Stack*)(&spu) + 1;
	reg_var = (double*)(spu_var) + reg_num;
	StackPop(&(spu.stack), reg_var);

	)
#undef StackPop
// DEF_CMD(COS, 10, ...)