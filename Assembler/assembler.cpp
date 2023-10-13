#include "assembler.h"

#define DEF_CMD(name, code, ...) if (strcmp(type, #name) == 0){ *type_code = code; is_finded = true;}

LogFile loggerASM;



int ASMCtor(ASM * asm_ptr, FileInfo * file) {

	LogFileCtor("ASM_log.log", &loggerASM);
	asm_ptr->CS = (char*)calloc(file->n_lines * (sizeof(double) + sizeof(int)), sizeof(char));
	asm_ptr->CS_size = 0;
	char* CS_ptr = asm_ptr->CS;
	for (int i = 0; i < file->n_lines; i++) {
		ASMDump(asm_ptr);
		double value = 0;
		int reg_num = 0;
		int type_code = 0;
		char command[COMMAND_SIZE] = {};

		int n = AssemblyCommand(file->text[i], &value, &reg_num, &type_code);
		if (n == ERROR) {
			fprintf(loggerASM.file, "Unknown command in %d line", i + 1);
			LogFileDtor(&loggerASM);
			return ERROR;
		}

		CSInsert(n, &(CS_ptr), &value, &reg_num, &type_code);
		if ((n == IMM) || (n == REG)) asm_ptr->CS_size += 2;
		if (n == EMPTY) asm_ptr->CS_size += 1;
	}
	ASMDump(asm_ptr);
	return 0;
}

int ASMDtor(ASM* asm_ptr) {
	free(asm_ptr->CS);
	asm_ptr->CS_size = 0;
	asm_ptr->ip = 0;
	return 0;
}

//int ASMVerify(ASM * asm_ptr);
int ASMDump(ASM* asm_ptr) {
	size_t iter = 0;
	char* asm_cs_var = asm_ptr->CS;

	printf("\n------------------------\n");
	while (iter < asm_ptr->CS_size) {
		Opcode opcode = *(Opcode*)((int*)(asm_cs_var));

		if (iter == asm_ptr->ip) printf(" [%d] ", *(int*)(asm_cs_var));
		else printf(" %d ", *(int*)(asm_cs_var));

		asm_cs_var = (char*)((int*)(asm_cs_var) + 1);
		
		iter++;

		if ((opcode.code == PUSH) || (opcode.code == POP)) {

			if (opcode.arg_type == 2) {

				if (iter == asm_ptr->ip) printf(" [%d] ", *(int*)(asm_cs_var));
				else printf(" %d ", *(int*)(asm_cs_var));
				asm_cs_var = (char*)((int*)asm_cs_var + 1);
			}
			else {

				if (iter == asm_ptr->ip) printf(" [%lf] ", *(double*)(asm_cs_var));
				else printf(" %lf ", *(double*)(asm_cs_var));
				asm_cs_var = (char*)((double*)asm_cs_var + 1);
			}
			
			iter++;
		}
	}
	printf("\n------------------------\n");
	return 0;
}


int WriteByteCode(ASM* asm_ptr, const char* filename) {
	FILE* res = {};
	fopen_s(&res, filename, "wb");
	char* asm_cs_var = asm_ptr->CS;
	while (asm_ptr->ip < asm_ptr->CS_size) {
		fwrite((int*)(asm_cs_var), sizeof(int), 1, res);
		Opcode opcode = *(Opcode*)((int*)(asm_cs_var));
		asm_cs_var = (char*)((int*)(asm_cs_var)+1);
		asm_ptr->ip++;

		if (opcode.arg_type == 1) {
			fwrite((double*)(asm_cs_var), sizeof(double), 1, res);
			asm_cs_var = (char*)((double*)asm_cs_var + 1);
			asm_ptr->ip++;
		}

		if (opcode.arg_type == 2) {
			fwrite((int*)(asm_cs_var), sizeof(int), 1, res);
			asm_cs_var = (char*)((int*)asm_cs_var + 1);
			asm_ptr->ip++;
		}
	}
	fclose(res);
	return 0;
}

int CSInsert(int arg_type,char** CS, double* value, int* reg_num, int* command) {

	*(int*)*CS = *command;
	*CS = (char*)((int*)*CS + 1);

	switch (arg_type) {
	case IMM:
		*(double*)*CS = *value;
		*CS = (char*)((double*)*CS + 1);
		break;
	case REG:
		*(int*)*CS = *reg_num;
		*CS = (char*)((int*)*CS + 1);
		break;
	}
	return 0;
}
int DetermineReg(char* str) {
	if ((strlen(str) == REG_SIZE) && (str[0] = 'R') && (str[2] = 'X')) {
		return str[1] - 'A';
	}
	else return ERROR;
}

int AssemblyCommand(char* str, double* value, int* reg_num, int* type_code) {

	assert(str != 0);
	char type[COMMAND_SIZE];

	// %[]
	// %n
	// 
	int arg_count = sscanf(str, "%s %lf", &type, value);

	if (arg_count <= 0) return ERROR;
	bool is_finded = false;
	// find command ( check define )
	#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
	if (is_finded = false) return ERROR;

	if ((*type_code == PUSH)||(*type_code == POP)) {
		if (arg_count == 1) {
			if (*type_code == PUSH) *type_code = PUSH + pow(2, 6) * 2;
			if (*type_code == POP) *type_code = POP + pow(2, 6) * 2;

			
			char reg_name[COMMAND_SIZE] = {};
			
			int arg_count2 = sscanf(str, "%s %s", &type, &reg_name);

			// TODO: Write in logger
			if (arg_count2 < 2) return ERROR;
			*reg_num = DetermineReg(reg_name);
			
			// TODO: Write in logger
			if (*reg_num == ERROR) return ERROR;
			*value = -1;
			return REG;
		}
		else {
			if (*type_code == PUSH) *type_code = PUSH + pow(2, 6) * 1;
			if (*type_code == POP) *type_code = POP + pow(2, 6) * 1;
			return IMM;
		}
	}

	return EMPTY;
}
