#include "SPU.h"

int SPUDump(SPU* spu) {
	StackDump(&(spu->stack));
	fprintf((spu->stack).logger.file, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	return 0;
}

int SPUVerify(SPU* spu) {
	return 0;
}

int SPUCtor(SPU* spu) {
	StackCtor(&(spu->stack), 2, "CPUlogger.log");
	spu->rax = 2;
	spu->rbx = 0;
	spu->rcx = 0;
	spu->rdx = 0;

	return 0;
}

int SPUDtor(SPU* spu) {
	StackDtor(&(spu->stack));
	return 0;
}

Reg_t* DetermineReg(char* str, SPU* spu) {
	if ((strlen(str) == REG_SIZE) && (str[0] = 'R') && (str[0] = 'X')) {
		int reg_num = str[1] - 'A';

		switch (reg_num){
		case 0:
			return &(spu->rax);
		case 1:
			return &(spu->rbx);
		case 2:
			return &(spu->rcx);
		case 3:
			return &(spu->rdx);
		}
	}
	return 0;
}

int ReadCommand(char* str, Opcode* opcode, double* value){
	int code = 0;
	int count = sscanf_s(str, "%d %lf", &code, value);

	*opcode = *(Opcode*)(&code);
	if ((count == 1) && (opcode->code != PUSH)) return 0;
	if ((count == 2) && (opcode->code == PUSH)) return 0;
	return -1;
}

#define DEF_CMD(name, code, handle) case name: handle; break;

int ExecuteProgramm(FileInfo* file, FILE* out) {
	SPU spu = {};
	SPUCtor(&spu);
	SPUDump(&spu);
	for (int i = 0; i < file->n_lines; i++) {
		Opcode opcode = {};
		double value = 0;
		if (ReadCommand(file->text[i], &opcode, &value) == -1) {
			fprintf(stderr, "Error in %d command", i + 1);
			return -1;
		}
		opcode;
		value;
		double a = 0;
		double b = 0;
		double in_var = 0;

		SPUDump(&spu);
		switch (opcode.code)
		{
		#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
		default:
			fprintf(stderr, "Error in %d command", i + 1);
			return -1;
			break;
		}
	}
}