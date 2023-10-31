#define _CRT_SECURE_NO_WARNINGS
#include "SPU.h"




int SPUDump(SPU* spu) {
	StackDump(&(spu->stack));
	printf("rax = %f\n", spu->rax);
	printf("rbx = %f\n", spu->rbx);
	printf("rcx = %f\n", spu->rcx);
	printf("rdx = %f\n", spu->rdx);
	
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
	spu->rax = 0;
	spu->rbx = 0;
	spu->rcx = 0;
	spu->rdx = 0;
	return 0;
}


int CSInsert(CS* cs, FileInfo* file) {
	fread(cs->CS, sizeof(char), file->buff_size, file->input_file);
	return 0;
}

int ReadCommand(CS* cs, size_t* ip, Opcode* opcode){
	*opcode = *(Opcode*)((int*)(cs->CS + *ip));
	*ip += sizeof(int);
	return 0;
}

int ReadRegArg(CS* cs, size_t* ip, int* reg_num) {
	*reg_num = *(int*)(cs->CS + *ip);
	*ip += sizeof(int);
	return 0;
}

int ReadImmArg(CS* cs, size_t* ip, double* value) {
	*value = *(double*)(cs->CS + *ip);
	*ip += sizeof(double);
	return 0;
}

int ReadArg(CS* cs, size_t* ip, Opcode opcode, double* value, int* reg_num) {
	if ((opcode.code == PUSH) || (opcode.code == POP) || (opcode.code == JMP)
		|| ((opcode.code >= JA) && (opcode.code <= JNE)) || (opcode.code == CALL)) {
		if (opcode.arg_type == REG) {
			ReadRegArg(cs, ip, reg_num);
		}
		else if (opcode.arg_type == PUSH) {
			ReadImmArg(cs, ip, value);
		}
		else {
			ReadImmArg(cs, ip, value);
		}
	}
	return 0;
}

#define DEF_CMD(name, code, handle) case name: handle; break;

int ExecuteProgramm(CS* cs, FILE* out) {
	SPU spu = {};
	SPUCtor(&spu);
	size_t line_iter = 0;

	while (cs->ip < cs->capacity) {
		SPUVerify(&spu);
		SPUDump(&spu);
		CSDump(cs);

		Opcode opcode;
		ReadCommand(cs, &cs->ip, &opcode);
		
		double value   = 0;
		int    reg_num = 0;
		ReadArg(cs, &cs->ip, opcode, &value, &reg_num);

		double a      = 0;
		double b      = 0;
		double in_var = 0;

		Stack*  spu_var = nullptr;
		double* reg_var = nullptr;

		switch (opcode.code)
		{
		#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
		default:
			SPUDump(&spu);
			fprintf(stderr, "Error in %d command", line_iter + 1);
			return -1;
			break;
		}
		line_iter++;
	}
	SPUDtor(&spu);

}