#define StackPop(a, b) if (StackPop(a, b) == -1) { fprintf(stderr, "Error in %d command", line_iter + 1); return -1;}

DEF_CMD(PUSH, 1,
	if (opcode.arg_type == IMM) StackPush(&(spu.stack), value);
	else if (opcode.arg_type == REG) {
		spu_var = (Stack*)(&spu) + 1;
		reg_var = (double*)(spu_var)+reg_num;
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
scanf("%lf", &in_var);
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
	if (opcode.arg_type == IMM) {
		fprintf(stderr, "IMM in POP argument");
		return -1;
	}
spu_var = (Stack*)(&spu) + 1;
reg_var = (Reg_t*)((Reg_t*)(spu_var)+reg_num);

if (reg_num < REG_COUNT) {
	StackPop(&(spu.stack), reg_var);
}
else {
	fprintf(stderr, "Undefined register");
	return -1;
}

)
DEF_CMD(JMP, 11,
	cs->ip = value;
)

#define DEF_JMP(name, cmp, num) DEF_CMD(name, num,\
							StackPop(&(spu.stack), &b);\
							StackPop(&(spu.stack), &a);\
							if (b cmp a) cs->ip = value;)
#include "def_jmp.h"
#undef DEF_JMP

DEF_CMD(CALL, 18,
	StackPush(&(spu.CallStack), cs->ip);
	cs->ip = value;
	)

DEF_CMD(RET, 19,
	StackPop(&(spu.CallStack), &a);
	cs->ip = a;
)

#undef StackPop
// DEF_CMD(COS, 10, ...)