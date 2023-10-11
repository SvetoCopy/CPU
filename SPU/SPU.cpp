#include "SPU.h"

int SPUDump(SPU* spu) {
	StackDump(&(spu->stack));
	size_t iter = 0;
	char* spu_cs_var = spu->CS;
	printf("\n------------------------\n");
	while (iter < spu->CS_size) {
		Opcode opcode = *(Opcode*)((int*)(spu_cs_var));

		if (iter == spu->ip) printf(" [%d] ", *(int*)(spu_cs_var));
		else printf(" %d ", *(int*)(spu_cs_var));

		spu_cs_var = (char*)((int*)(spu_cs_var) + 1);
		
		iter++;

		if ((opcode.code == PUSH) || (opcode.code == POP)) {

			if (opcode.arg_type == 2) {

				if (iter == spu->ip) printf(" [%d] ", *(int*)(spu_cs_var));
				else printf(" %d ", *(int*)(spu_cs_var));
				spu_cs_var = (char*)((int*)spu_cs_var + 1);
			}
			else {

				if (iter == spu->ip) printf(" [%lf] ", *(double*)(spu_cs_var));
				else printf(" %lf ", *(double*)(spu_cs_var));
				spu_cs_var = (char*)((double*)spu_cs_var + 1);
			}
			
			iter++;
		}
	}
	printf("\n------------------------\n");
	return 0;
}

int SPUVerify(SPU* spu) {
	return 0;
}

int SPUCtor(SPU* spu, FileInfo* file) {
	StackCtor(&(spu->stack), 2, "CPUlogger.log");
	spu->rax = 2;
	spu->rbx = 0;
	spu->rcx = 0;
	spu->rdx = 0;
	spu->CS = (char*)calloc(file->n_lines*(sizeof(double) + sizeof(int)), sizeof(char));
	size_t CS_iter = 0;
	char*  CS_ptr = spu->CS;

	for (int i = 0; i < file->n_lines; ++i) {
		int command = 0;
		double val = 0;
		int reg_num = -1;
		int    n = ReadCommand(file->text[i], &command, &val, &reg_num);
		// TODO
		if (n == -1) {
			fprintf(stderr, "Error in %d command", i + 1);
			return -1;
		}
		if (n == 2) {

			*(int*)CS_ptr = command;
			CS_ptr = (char*)((int*)CS_ptr + 1);
			CS_iter++;

			if (reg_num == -1) {
				*(double*)CS_ptr = val;
				CS_ptr = (char*)((double*)CS_ptr + 1);
			}
			else {
				*(int*)CS_ptr = reg_num;
				CS_ptr = (char*)((int*)CS_ptr + 1);
			}
			CS_iter++;
		}
		else {
			*(int*)CS_ptr = command;
			CS_ptr = (char*)((int*)CS_ptr + 1);
			CS_iter++;
		}
	
	}
	spu->CS_size = CS_iter;
	return 0;
}

int SPUDtor(SPU* spu) {
	StackDtor(&(spu->stack));
	return 0;
}



int ReadCommand(char* str, int* command, double* value, int* reg_num){

	int count = sscanf_s(str, "%d %lf", command, value);
	Opcode opcode = *(Opcode*)(command);


	if ((count == 1) && (opcode.code != PUSH) && (opcode.code != POP)) return 1;
	if ((count == 2) && ((opcode.code == PUSH) || (opcode.code == POP))) {
		if (opcode.arg_type == 2) {
			*reg_num = (int)*value;
			*value = 0;
		}
		return 2;
	}
	return -1;
}

#define DEF_CMD(name, code, handle) case name: handle; break;

// TODO: 2, 1 - magic const
int ExecuteProgramm(FileInfo* file, FILE* out) {
	SPU spu = {};
	SPUCtor(&spu, file);
	size_t line_iter = 0;
	char* spu_cs_var = spu.CS;
	while (spu.ip < spu.CS_size) {
		SPUDump(&spu);

		Opcode opcode = *(Opcode*)((int*)(spu_cs_var));
		spu_cs_var = (char*)((int*)(spu_cs_var)+1);

		spu.ip++;
		
		double value = 0;
		int reg_num = 0;
		if ((opcode.code == PUSH) || (opcode.code == POP)) {
			if (opcode.arg_type == 2) {
				reg_num = *(int*)(spu_cs_var);
				spu_cs_var = (char*)((int*)spu_cs_var + 1);
			}
			else {
				value = *(double*)(spu_cs_var);
				spu_cs_var = (char*)((double*)spu_cs_var + 1);
			}
			spu.ip++;
		}

		double a = 0;
		double b = 0;
		double in_var = 0;

		Stack* spu_var = nullptr;
		double* reg_var = nullptr;

		switch (opcode.code)
		{
		#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
		default:
			fprintf(stderr, "Error in %d command", line_iter + 1);
			return -1;
			break;
		}
		line_iter++;
	}

}