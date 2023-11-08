#define StackPop(a, b) if (StackPop(a, b) == -1) { fprintf(stderr, "Error in %d command", command_iter + 1); return -1;}
// DEF_CMD(name, code, args_num, handle)

DEF_CMD(PUSH, 1, 1,
	Value_t * value = (Value_t*)ReadArg(spu, cs, &cs->ip, opcode.arg_type);
	StackPush(&(spu->stack), *value);
)

DEF_CMD(DIV, 2, 0,
	Value_t a = 0;
	Value_t b = 0;
	StackPop(&(spu->stack), &b);
	StackPop(&(spu->stack), &a);

	StackPush(&(spu->stack), a / b);
)

DEF_CMD(SUB, 3, 0,
	Value_t a = 0;
	Value_t b = 0;
	StackPop(&(spu->stack), &b);
	StackPop(&(spu->stack), &a);
	StackPush(&(spu->stack), a - b);
)

DEF_CMD(OUT, 4, 0,
	Value_t a = 0;
	StackPop(&(spu->stack), &a);
	printf("OUT = %f\n", a);
	fprintf(out, "OUT = %f\n", a);
)
DEF_CMD(IN, 5, 0,
	Value_t in_var = 0;
	printf(">");
	scanf("%lf", &in_var);
	StackPush(&(spu->stack), in_var);
)
DEF_CMD(MUL, 6, 0,
	Value_t a = 0;
	Value_t b = 0;
	StackPop(&(spu->stack), &b);
	StackPop(&(spu->stack), &a);
	StackPush(&(spu->stack), a * b);
)
DEF_CMD(SIN, 7, 0,
	Value_t a = 0;
	StackPop(&(spu->stack), &a);
	StackPush(&(spu->stack), sin(a));
)
DEF_CMD(COS, 8, 0,
	Value_t a = 0;
	StackPop(&(spu->stack), &a);
	StackPush(&(spu->stack), cos(a));
	)


DEF_CMD(HLT, 9, 0,
	return 0;
)
DEF_CMD(POP, 10, 1,
	if (opcode.arg_type == IMM) {
		fprintf(stderr, "IMM in POP argument");
		return -1;
	}
	double ret_value = 0;
	StackPop(&(spu->stack), &ret_value);
	
	if (opcode.arg_type == REG) {
		Reg_t* value = (Reg_t*)ReadArg(spu, cs, &cs->ip, opcode.arg_type);
		*value = ret_value;
	}
	if ((opcode.arg_type == RAM_IMM) || (opcode.arg_type == RAM_REG)) {
		RAM_t* value = (RAM_t*)ReadArg(spu, cs, &cs->ip, opcode.arg_type);
		*value = (RAM_t)ret_value;
	}
)
DEF_CMD(JMP, 11, 1,
	Value_t* value = (Value_t*)ReadArg(spu, cs, &cs->ip, opcode.arg_type);
	cs->ip = *value;
)

#define DEF_JMP(name, cmp, num) DEF_CMD(name, num, 1,							        \
							Value_t* value = (Value_t*)ReadArg(spu, cs, &cs->ip, opcode.arg_type); \
							Value_t a = 0;												\
							Value_t b = 0;												\
							StackPop(&(spu->stack), &b);						        \
							StackPop(&(spu->stack), &a);						        \
							if (b cmp a) cs->ip = *value;)
#include "def_jmp.h"
#undef DEF_JMP

DEF_CMD(CALL, 18, 1,
	Value_t* value = (Value_t*)ReadArg(spu, cs, &cs->ip, opcode.arg_type);
	StackPush(&(spu->CallStack), cs->ip);
	cs->ip = *value;
	)

DEF_CMD(RET, 19, 0,
	Value_t a = 0;
	StackPop(&(spu->CallStack), &a);
	cs->ip = a;
)
DEF_CMD(ADD, 20, 0,
	Value_t a = 0;
	Value_t b = 0;
	StackPop(&(spu->stack), &b);
	StackPop(&(spu->stack), &a);
	StackPush(&(spu->stack), a + b);
	)
DEF_CMD(SQRT, 21, 0,
	Value_t a = 0;
	StackPop(&(spu->stack), &a);
	StackPush(&(spu->stack), sqrt(a));
)
DEF_CMD(PRINT, 22, 0,
	VRamPrint(spu);
)
#undef StackPop