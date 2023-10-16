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
	StackVerify(&(spu->stack));
	return 0;
}


int SPUCtor(SPU* spu) {
	StackCtor(&spu->stack, 2, "SPU_Stack_Dump.log");
	LogFileCtor("SPU_Dump.log", &spu->logfile);

	spu->rax = 0;
	spu->rbx = 0;
	spu->rcx = 0;
	spu->rdx = 0;
	
	return 0;
}

int SPUDtor(SPU* spu) {
	StackDtor(&(spu->stack));
	spu->rax = 0;
	spu->rbx = 0;
	spu->rcx = 0;
	spu->rdx = 0;
	return 0;
}


int CSInsert(char** CS, FileInfo* file) {
	fread(*CS, sizeof(char), file->buff_size, file->input_file);
	return 0;
}

int ReadCommand(char** CS_ptr, size_t* ip, Opcode* opcode){
	*opcode = *(Opcode*)((int*)(*CS_ptr));
	*CS_ptr = (char*)((int*)(*CS_ptr)+1);

	*ip += sizeof(int);
	return 0;
}

int ReadRegArg(char** CS_ptr, size_t* ip, int* reg_num) {
	*reg_num = *(int*)(*CS_ptr);
	*CS_ptr = (char*)((int*)*CS_ptr + 1);

	*ip += sizeof(int);
	return 0;
}

int ReadImmArg(char** CS_ptr, size_t* ip, double* value) {
	*value = *(double*)(*CS_ptr);
	*CS_ptr = (char*)((double*)*CS_ptr + 1);

	*ip += sizeof(double);
	return 0;
}

int ReadArg(char** CS_ptr, size_t* ip, Opcode opcode, double* value, int* reg_num) {
	if ((opcode.code == PUSH) || (opcode.code == POP)) {
		if (opcode.arg_type == REG) {
			ReadRegArg(CS_ptr, ip, reg_num);
		}
		else {
			ReadImmArg(CS_ptr, ip, value);
		}
	}
	return 0;
}

#define DEF_CMD(name, code, handle) case name: handle; break;

int ExecuteProgramm(CS* cs, FILE* out) {
	SPU spu = {};
	SPUCtor(&spu);
	size_t line_iter = 0;
	char*  cs_var    = cs->CS;

	while (cs->ip < cs->capacity) {
		SPUVerify(&spu);
		SPUDump(&spu);
		CSDump(cs);

		Opcode opcode;
		ReadCommand(&cs_var, &cs->ip, &opcode);
		
		double value   = 0;
		int    reg_num = 0;
		ReadArg(&cs_var, &cs->ip, opcode, &value, &reg_num);

		double a = 0;
		double b = 0;
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