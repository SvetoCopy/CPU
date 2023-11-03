#define StackPop(a, b) if (StackPop(a, b) == -1) { fprintf(stderr, "Error in %d command", line_iter + 1); return -1;}
// DEF_CMD(name, code, args_num, handle)
DEF_CMD(PUSH, 1, 1,
	StackPush(&(spu->stack), value);
)

DEF_CMD(DIV, 2, 0,

	StackPop(&(spu->stack), &b);
StackPop(&(spu->stack), &a);

StackPush(&(spu->stack), a / b);
)

DEF_CMD(SUB, 3, 0,
	StackPop(&(spu->stack), &b);
StackPop(&(spu->stack), &a);
StackPush(&(spu->stack), a - b);
)

DEF_CMD(OUT, 4, 0,
	StackPop(&(spu->stack), &a);
printf("OUT = %f\n", a);
fprintf(out, "OUT = %f\n", a);
)
DEF_CMD(IN, 5, 0,
	printf(">");
scanf("%lf", &in_var);
StackPush(&(spu->stack), in_var);
)
DEF_CMD(MUL, 6, 0,
	StackPop(&(spu->stack), &b);
StackPop(&(spu->stack), &a);
StackPush(&(spu->stack), a * b);
)
DEF_CMD(SIN, 7, 0,
	StackPop(&(spu->stack), &a);
StackPush(&(spu->stack), sinf(a));
)
DEF_CMD(COS, 8, 0,
	StackPop(&(spu->stack), &a);
StackPush(&(spu->stack), cosf(a));
)


DEF_CMD(HLT, 9, 0,
	return 0;
)
DEF_CMD(POP, 10, 1,
	if (opcode.arg_type == IMM) {
		fprintf(stderr, "IMM in POP argument");
		return -1;
	}
	StackPop(&(spu->stack), value_to_change);
)
DEF_CMD(JMP, 11, 1,
	cs->ip = value;
)

#define DEF_JMP(name, cmp, num) DEF_CMD(name, num, 1,   \
							StackPop(&(spu->stack), &b);\
							StackPop(&(spu->stack), &a);\
							if (b cmp a) cs->ip = value;)
#include "def_jmp.h"
#undef DEF_JMP

DEF_CMD(CALL, 18, 1,
	StackPush(&(spu->CallStack), cs->ip);
	cs->ip = value;
	)

DEF_CMD(RET, 19, 0,
	StackPop(&(spu->CallStack), &a);
	cs->ip = a;
)
DEF_CMD(ADD, 20, 0,
	StackPop(&(spu->stack), &b);
	StackPop(&(spu->stack), &a);
	StackPush(&(spu->stack), a + b);
	)
DEF_CMD(SQRT, 21, 0,
StackPop(&(spu->stack), &a);
StackPush(&(spu->stack), sqrt(a));
)

#undef StackPop
// DEF_CMD(COS, 10, ...)