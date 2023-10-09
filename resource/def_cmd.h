#define StackPop(a, b) if (StackPop(a, b) == -1) { fprintf(stderr, "Error in %d command", i + 1); return -1;}

DEF_CMD(PUSH, 1, 
	//* (pow(10, FACTOR)
	StackPush(&(spu.stack), (int)(command.value * (pow(10, FACTOR))));
	)

DEF_CMD(DIV, 2, 

	StackPop(&(spu.stack), &b1); 
	StackPop(&(spu.stack), &a1);
	a2 = a1;
	b2 = b1;
	StackPush(&(spu.stack), (int)((a2 / b2) * (pow(10, FACTOR))));
	)

DEF_CMD(SUB, 3, 
	StackPop(&(spu.stack), &b1); 
	StackPop(&(spu.stack), &a1); 
	a2 = a1;
	b2 = b1;
	StackPush(&(spu.stack), a2 - b2);
	)

DEF_CMD(OUT, 4, 
	StackPop(&(spu.stack), &a1); 
	a2 = a1;
	printf("OUT = %f\n", a2 / (pow(10, FACTOR)));
	fprintf(out, "OUT = %f\n", a2 / (pow(10, FACTOR)));
	)
DEF_CMD(IN, 5,
	printf(">");
	scanf_s("%d", &n);
	StackPush(&(spu.stack), n);
	)
DEF_CMD(MUL, 6,
	StackPop(&(spu.stack), &b1);
	StackPop(&(spu.stack), &a1);
	a2 = a1;
	b2 = b1;
	StackPush(&(spu.stack), a1 * b1 / (pow(10, FACTOR)));
)
DEF_CMD(SIN, 7,
	StackPop(&(spu.stack), &a1);
	a2 = a1;
	StackPush(&(spu.stack), int(sinf(a2 / (pow(10, FACTOR))) * (pow(10, FACTOR))));
)
DEF_CMD(COS, 8,
	StackPop(&(spu.stack), &a1);
	a2 = a1;
	StackPush(&(spu.stack), int(cosf(a2 / (pow(10, FACTOR))) * (pow(10, FACTOR))));
)

DEF_CMD(HLT, -2, 
	return 0;
	)
#undef StackPop
// DEF_CMD(COS, 10, ...)