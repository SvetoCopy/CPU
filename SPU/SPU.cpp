#define _CRT_SECURE_NO_WARNINGS
#include "SPU.h"

int VRamPrint(SPU* spu) {

	SPUVerify(spu);

	for (int i = 0; i < VRAM_LEN; i++) {
		if (i % VRAM_WIDTH == 0) printf("\n");
		printf("%c", (char)spu->RAM[i]);
	}

	printf("\n");

	return 0;
}

int SPUDump(SPU* spu) {

	SPUVerify(spu);

	printf("------------------------------------\n");

	#define DEF_REG(name, ...) printf(#name "[%p] = %f\n", &(spu->name), spu->name);
	#include "..\resource\def_reg.h"
	#undef DEF_REG

	StackDump(&(spu->stack));

	printf("RAM[%p]:\n", &spu->RAM);

	for (int i = 0; i < VISIBLE_RAM_PART; i++) {
		printf("[%d] = %lf\n", i, spu->RAM[i]);
	}

	printf("------------------------------------\n");

	return 0;
}

int SPUVerify(SPU* spu) {

	if (spu == nullptr) assert(!"SPU is null");

	StackVerify(&spu->stack);
	StackVerify(&(spu->call_stack));

	if (spu->RAM == nullptr) fprintf(stderr, "RAM is null");

	return 0;
}

int SPUCtor(SPU* spu) {

	int stack_capacity = 2;

	StackCtor(&spu->stack, stack_capacity, "SPU_Stack_Dump.log");
	StackCtor(&spu->call_stack, stack_capacity, "SPU_Stack_Dump1.log");

	LogFileCtor("SPU_Dump.log", &spu->logfile);
	spu->RAM = (Value_t*)calloc(RAM_LEN, sizeof(Value_t));

	#define DEF_REG(name, ...) spu->name = 0;
	#include "..\resource\def_reg.h"
	#undef DEF_REG
	
	return 0;
}

int SPUDtor(SPU* spu) {

	StackDtor(&spu->stack);
	StackDtor(&spu->call_stack);

	free(spu->RAM);

	#define DEF_REG(name, ...) spu->name = 0;
	#include "..\resource\def_reg.h"
	#undef DEF_REG

	return 0;
}

Value_t* GetRegisterPtr(SPU* spu, int reg_num) {

	SPUVerify(spu);

	#define DEF_REG(reg_name, reg_code) if (reg_code == reg_num) return &spu->reg_name;
	#include "..\resource\def_reg.h"
}

int ReadCommand(CodeSeg* cs, size_t* ip, Opcode* opcode) {

	CodeSegVerify(cs);

	*opcode = *(Opcode*)(cs->CodeSeg_ptr + *ip);
	*ip    += SIZEOF_COMMAND;

	return 0;
}

Value_t* ReadRegArg(SPU* spu, CodeSeg* cs, size_t* ip) {

	SPUVerify(spu);
	CodeSegVerify(cs);

	int      address_num = *(int*)(cs->CodeSeg_ptr + *ip);
	Value_t* value       = GetRegisterPtr(spu, address_num);

	*ip += SIZEOF_ADDRESS_NUM;
	
	return value;
}

Value_t* ReadImmArg(CodeSeg* cs, size_t* ip) {

	CodeSegVerify(cs);

	double* value = (double*)(cs->CodeSeg_ptr + *ip);

	*ip  += SIZEOF_VALUE;

	return value;
}

Value_t* ReadRamReg(SPU* spu, CodeSeg* cs, size_t* ip) {

	SPUVerify(spu);
	CodeSegVerify(cs);

	int      address_num = *(int*)(cs->CodeSeg_ptr + *ip);
	double   reg_value   = *GetRegisterPtr(spu, address_num);
	Value_t* value       = &(spu->RAM[(int)reg_value]);

	*ip += SIZEOF_ADDRESS_NUM;
	
	return value;
}

Value_t* ReadRamImm(SPU* spu, CodeSeg* cs, size_t* ip) {

	CodeSegVerify(cs);
	SPUVerify(spu);

	int      address_num = *(int*)(cs->CodeSeg_ptr + *ip);
	Value_t* value       = &(spu->RAM[address_num]);

	*ip += SIZEOF_ADDRESS_NUM;
	
	return value;
}

Value_t* ReadArg(SPU* spu, CodeSeg* cs, size_t* ip, int arg_type) {

	CodeSegVerify(cs);
	SPUVerify(spu);

	// RAM рассмотреть как случай REG
	if (arg_type == REG)     return ReadRegArg(spu, cs, ip);
	if (arg_type == IMM)     return ReadImmArg(cs, ip);
	if (arg_type == RAM_IMM) return ReadRamImm(spu, cs, ip);
	if (arg_type == RAM_REG) return ReadRamReg(spu, cs, ip);

	return 0;
}

int ExecuteProgram(SPU* spu, CodeSeg* cs, FILE* out) {

	CodeSegVerify(cs);
	SPUVerify(spu);

	size_t command_iter = 0;
	int print_count = 0;
	while (spu->ip < cs->capacity) {
		//printf("command: %zu\n", command_iter);
		//SPUVerify(spu);
		//CodeSegVerify(cs);

		//CodeSegDump(cs, spu->ip);

		Opcode opcode = {};
		ReadCommand(cs, &spu->ip, &opcode);

		switch (opcode.code)
		{
		#define DEF_CMD(name, code, args_count, handle) { case name: handle; break; }
		#include "..\resource\def_cmd.h"
		#undef DEF_CMD
		default:
			SPUDump(spu);
			fprintf(stderr, "UNKOWN COMMAND: Error in %zu command", command_iter + 1);

			return ERROR;
		}
		
		command_iter++;
		//SPUDump(spu);
	}
	
	//CodeSegDump(cs, spu->ip);
	SPUDtor(spu);

	return 0;
}