#define STACK_POP(ptr)                                               \
        if (StackPop(&spu->stack, ptr) == STACK_ERROR) {             \
			fprintf(stderr, "Error in %d command: StackERROR in POP", command_iter + 1);\
			return SPU_ERROR;                                        \
		}

#define STACK_PUSH(value)                                            \
		StackPush(&spu->stack, value);

#define CALLBACK_STACK_POP(ptr)                                      \
        if (StackPop(&spu->call_stack, ptr) == STACK_ERROR) {        \
			fprintf(stderr, "Error in %d command: : CallBackStackERROR in POP", command_iter + 1);\
			return SPU_ERROR;                                        \
		}

#define CALLBACK_STACK_PUSH(value)                                   \
        StackPush(&spu->call_stack, value);

// name --+      +-- arg_num  
//        | code |
//        |   |  |
//        V   V  V                  handle -----------------------------+
DEF_CMD(PUSH, 1, 1,                                               //    |
	Value_t* value = ReadArg(spu, cs, &spu->ip, opcode.arg_type); //<---+ 
	STACK_PUSH(*value);											  //
)

DEF_CMD(DIV, 2, 0,
	Value_t first  = 0;
	Value_t second = 0;

	STACK_POP(&second);
	STACK_POP(&first);

	assert(second != 0 && "Error: Division by zero");

	STACK_PUSH(first / second);
)

DEF_CMD(SUB, 3, 0,
	Value_t first = 0;
	Value_t second = 0;

	STACK_POP(&second);
	STACK_POP(&first);

	STACK_PUSH(first - second);
)

DEF_CMD(OUT, 4, 0,
	Value_t out_value = 0;

	STACK_POP(&out_value);

	printf("OUT = %f\n", out_value);
	fprintf(out, "OUT = %f\n", out_value);
)

DEF_CMD(IN, 5, 0,
	Value_t in_var = 0;

	printf(">");
	scanf("%lf", &in_var);

	STACK_PUSH(in_var);
)

DEF_CMD(MUL, 6, 0,
	Value_t first = 0;
	Value_t second = 0;

	STACK_POP(&second);
	STACK_POP(&first);

	STACK_PUSH(first * second);
)

DEF_CMD(SIN, 7, 0,
	Value_t arg = 0;

	STACK_POP(&arg);

	STACK_PUSH(sin(arg));
)

DEF_CMD(COS, 8, 0,
	Value_t arg = 0;

	STACK_POP(&arg);

	STACK_PUSH(cos(arg));
)

DEF_CMD(HLT, 9, 0,
	return 0;
)

DEF_CMD(POP, 10, 1,
	if (opcode.arg_type == IMM) {
		fprintf(stderr, "IMM in POP argument");
		return SPU_ERROR;
	}

	double ret_value = 0;

	STACK_POP(&ret_value);

	// get arg ptr
	Value_t* value = ReadArg(spu, cs, &spu->ip, opcode.arg_type);
	*value = ret_value;
)

DEF_CMD(JMP, 11, 1,
	Value_t* value = ReadArg(spu, cs, &spu->ip, opcode.arg_type);
	spu->ip = *value;
)

#define DEF_JMP(name, cmp, num) DEF_CMD(name, num, 1,							                      \
							Value_t* value = (Value_t*)ReadArg(spu, cs, &spu->ip, opcode.arg_type);   \
							Value_t first = 0;											              \
							Value_t second = 0;												          \
																									  \
							STACK_POP(&second);						                                  \
							STACK_POP(&first);						                                  \
                                                                                                      \
							if (second cmp first) spu->ip = *value;)

#include "def_jmp.h"

#undef DEF_JMP

DEF_CMD(CALL, 18, 1,
	Value_t* value = ReadArg(spu, cs, &spu->ip, opcode.arg_type);
	
	CALLBACK_STACK_PUSH(spu->ip);

	printf("CALLING to Func\n");

	spu->ip = *value;
)

DEF_CMD(RET, 19, 0,
	Value_t ret_value = 0;
	
	CALLBACK_STACK_POP(&ret_value);
	printf("RETURNING from Func\n");
	spu->ip = ret_value;
)

DEF_CMD(ADD, 20, 0,
	Value_t first = 0;
	Value_t second = 0;

	STACK_POP(&second);
	STACK_POP(&first);

	STACK_PUSH(first + second);
)

DEF_CMD(POW, 21, 0,
	Value_t first = 0;
	Value_t second = 0;

	STACK_POP(&second);
	STACK_POP(&first);

	STACK_PUSH(pow(first, second));
)

DEF_CMD(PRINT, 22, 0,
	Value_t sym = 0;

	STACK_POP(&sym);
	//printf("print: %c count: %d\n", (char)sym, print_count);
	//system("pause");
	fprintf(out, "%c", (char)sym);
	print_count++;
)

#undef StackPop