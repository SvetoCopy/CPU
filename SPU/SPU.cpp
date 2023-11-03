#define _CRT_SECURE_NO_WARNINGS
#include "SPU.h"

int DrawCircle(SPU* spu) {
	int height = GD_LEN / GD_WIDTH;
	int width = GD_WIDTH;
	int center_x = width / 2 + 1;
	int center_y = height / 2 + 1;
	int radius = height - center_y;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if ((x + 1 - center_x) * (x + 1 - center_x) + (y + 1 - center_x) * (y + 1 - center_x) <= radius * radius) spu->GD[y * width + x] = '*';
			else spu->GD[y * width + x] = ' ';
		}
	}
	return 0;
}

int GDPrint(SPU* spu) {
	for (int i = 0; i < GD_LEN; i++) {
		if (i % GD_WIDTH == 0) printf("\n");
		printf("%c", spu->GD[i]);
	}
	printf("\n");
	return 0;
}

int SPUDump(SPU* spu) {
	
	printf("------------------------------------\n");
	printf("rax[%p] = %f\n", &(spu->rax),spu->rax);
	printf("rbx[%p] = %f\n", &(spu->rbx), spu->rbx);
	printf("rcx[%p] = %f\n", &(spu->rcx), spu->rcx);
	printf("rdx[%p] = %f\n", &(spu->rdx), spu->rdx);
	printf("r8[%p] = %f\n", &(spu->r8), spu->r8);
	printf("r9[%p] = %f\n", &(spu->r9), spu->r9);
	printf("r10[%p] = %f\n\n", &(spu->r10), spu->r10);
	StackDump(&(spu->stack));
	printf("RAM[%p]:\n", &spu->RAM);
	for (int i = 0; i < 3; i++) {
		printf("[%d] = %lf\n", i, spu->RAM[i]);
	}
	//GDPrint(spu);
	printf("------------------------------------\n");
	return 0;
}

int SPUVerify(SPU* spu) {
	StackVerify(&spu->stack);
	StackVerify(&(spu->CallStack));
	return 0;
}


int SPUCtor(SPU* spu) {
	int stack_capacity = 2;
	StackCtor(&spu->stack, stack_capacity, "SPU_Stack_Dump.log");
	StackCtor(&spu->CallStack, stack_capacity, "SPU_Stack_Dump1.log");
	LogFileCtor("SPU_Dump.log", &spu->logfile);
	spu->RAM = (RAM_t*)calloc(RAM_LEN, sizeof(RAM_t));
	spu->GD = (GD_t*)calloc(GD_LEN, sizeof(GD_t));
	spu->labels_count = 0;
	spu->rax = 0;
	spu->rbx = 0;
	spu->rcx = 0;
	spu->rdx = 0;
	
	return 0;
}

int SPUDtor(SPU* spu) {
	StackDtor(&(spu->stack));
	StackDtor(&(spu->CallStack));
	free(spu->RAM);
	free(spu->GD);
	spu->rax = 0;
	spu->rbx = 0;
	spu->rcx = 0;
	spu->rdx = 0;
	return 0;
}



double GetRegisterValue(SPU* spu, int reg_num) {
	#define DEF_REG(reg_name, reg_code) if (reg_code == reg_num) return spu->reg_name;
	#include "..\resource\def_reg.h"
}

double* GetRegisterPtr(SPU* spu, int reg_num) {
	#define DEF_REG(reg_name, reg_code) if (reg_code == reg_num) return &(spu->reg_name);
	#include "..\resource\def_reg.h"
}

int ReadCommand(CS* cs, size_t* ip, Opcode* opcode){
	*opcode = *(Opcode*)((int*)(cs->CS + *ip));
	*ip += sizeof(int);
	return 0;
}

int ReadRegArg(SPU* spu, CS* cs, size_t* ip, int* address_num, double* value, double** value_to_change) {
	*address_num = *(int*)(cs->CS + *ip);
	*ip += sizeof(int);
	*value = GetRegisterValue(spu, *address_num);
	*value_to_change = GetRegisterPtr(spu, *address_num);
	return 0;
}

int ReadImmArg(CS* cs, size_t* ip, double* value) {
	*value = *(double*)(cs->CS + *ip);
	*ip += sizeof(double);
	return 0;
}

int ReadRAM_REG(SPU* spu, CS* cs, size_t* ip, int* address_num, double* value, double** value_to_change) {
	*address_num = *(int*)(cs->CS + *ip);
	*ip += sizeof(int);
	double reg_value = GetRegisterValue(spu, *address_num);
	*value = spu->RAM[(int)reg_value];
	*value_to_change = &(spu->RAM[(int)reg_value]);
	return 0;
}
int ReadRAM_IMM(SPU* spu, CS* cs, size_t* ip, int* address_num, double* value, double** value_to_change) {
	*address_num = *(int*)(cs->CS + *ip);
	*ip += sizeof(int);
	*value = spu->RAM[*address_num];
	*value_to_change = &(spu->RAM[*address_num]);
	return 0;
}

int ReadArg(SPU* spu, CS* cs,size_t* ip, Opcode opcode, double* value, int* address_num, double** value_to_change) {
	#define DEF_CMD(name, number, args_count, ...)                                                                \
			if (opcode.code == number){                                                                           \
				if (args_count == 1){                                                                             \
					if (opcode.arg_type == REG) ReadRegArg(spu, cs, ip, address_num, value, value_to_change);     \
					if (opcode.arg_type == IMM) ReadImmArg(cs, ip, value);                                        \
					if (opcode.arg_type == RAM_IMM) ReadRAM_IMM(spu, cs, ip, address_num, value, value_to_change);\
					if (opcode.arg_type == RAM_REG) ReadRAM_REG(spu, cs, ip, address_num, value, value_to_change);\
				}	                                                                                              \
				return 0;                                                                                         \
			}                                          
	#include "..\resource\def_cmd.h"
	return 0;
}

#define DEF_CMD(name, code, args_count, handle) case name: handle; break;

int ExecuteProgramm(SPU* spu, CS* cs, FILE* out) {
	size_t line_iter = 0;

	while (cs->ip < cs->capacity) {
		
		SPUVerify(spu);
		
		CSDump(cs);

		Opcode opcode;
		ReadCommand(cs, &cs->ip, &opcode);
		          
		double value           = 0;
		int    address_num     = 0;
		double* value_to_change = nullptr;
		ReadArg(spu, cs, &cs->ip, opcode, &value, &address_num, &value_to_change);

		double a      = 0;
		double b      = 0;
		double in_var = 0;
		
		switch (opcode.code)
		{
		#include "..\resource\def_cmd.h"
		default:
			SPUDump(spu);
			fprintf(stderr, "Error in %d command", line_iter + 1);
			return ERROR;
			break;
		}
		
		line_iter++;
		SPUDump(spu);
	}
	CSDump(cs);
	SPUDtor(spu);

}