#include "assembler.h"

LogFile loggerASM;

int WriteByteCode(CS* cs, const char* filename) {
	FILE* res = {};
	fopen_s(&res, filename, "wb");
	fwrite(cs->CS, sizeof(char), cs->size, res);
	fclose(res);
	return 0;
}

int CSInsertIntCode(CS* cs, int* code) {
	*(int*)(cs->CS) = *code;
	CSDump(cs);
	cs->CS = (char*)((int*)(cs->CS) + 1);
	cs->size += sizeof(int);
	return 0;
}

int CSInsertDoubleCode(CS* cs, double* code) {
	*(double*)(cs->CS) = *code;
	cs->CS = (char*)((double*)(cs->CS) + 1);
	cs->size += sizeof(double);
	return 0;
}

int CSInsert(int arg_type, CS* cs, double* value, int* reg_num, int* command) {
	printf("11\n");
	CSDump(cs);
	CSInsertIntCode(cs, command);
	CSDump(cs);
	switch (arg_type) {
	case IMM:
		CSInsertDoubleCode(cs, value);
		break;
	case REG:
		CSInsertIntCode(cs, reg_num);
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


int AssemblyReg(char* str, int* type_code, int* reg_num) {
	Opcode opcode_push = { PUSH, REG };

	if (*type_code == PUSH) {
		Opcode opcode = { PUSH, REG };
		*type_code = *(int*)(&opcode);
	}
	if (*type_code == POP) {
		Opcode opcode = { POP, REG };
		*type_code = *(int*)(&opcode);
	}
	if (*type_code == JMP) {
		Opcode opcode = { JMP, REG };
		*type_code = *(int*)(&opcode);
	}

	char reg_name[COMMAND_SIZE] = {};
	char type_str[COMMAND_SIZE] = {};
	int  arg_count2 = sscanf(str, "%s %s", &type_str, &reg_name);

	// TODO: Write in logger
	if (arg_count2 < 2) return ERROR;
	*reg_num = DetermineReg(reg_name);

	// TODO: Write in logger
	if (*reg_num == ERROR) return ERROR;
	
	return 0;
}

int AssemblyImm(char* str, int* type_code, double* value) {
	if (*type_code == PUSH) {
		Opcode opcode = { PUSH, IMM };
		*type_code = *(int*)(&opcode);
	}
	if (*type_code == POP) {
		Opcode opcode = { POP, IMM };
		*type_code = *(int*)(&opcode);
	}
	if (*type_code == JMP) {
		Opcode opcode = { JMP, IMM };
		*type_code = *(int*)(&opcode);
	}
	return 0;
}

int AssemblyArg(char* str, int* type_code, int arg_count, int* reg_num, double* value) {
	if (arg_count == 1) {
		int n = AssemblyReg(str, type_code, reg_num);
		*value = -1;

		// TODO: Write in logger
		if (n == ERROR) return ERROR;
		return REG;
	}
	else {
		int n = AssemblyImm(str, type_code, value);
		// TODO: Write in logger
		if (n == ERROR) return ERROR;
		return IMM;
	}
}

int AssemblyCommand(char* str, CS* cs) {

	assert(str != 0);

	double value     = 0;
	int    reg_num   = 0;
	int    type_code = 0;
	int    arg_type  = -1;
	
	char type_str[COMMAND_SIZE];

	// %[]
	// %n
	int arg_count = sscanf(str, "%s %lf", &type_str, &value);
	if (arg_count <= 0) return ERROR;

	#define DEF_CMD(name, code, ...) else if (strcmp(type_str, #name) == 0){ type_code = code;}
	if (0) {}
	#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
	#undef DEF_CMD
	else return ERROR;

	// write_cmd_code(CS, PUSH)
	if ((type_code == PUSH) || (type_code == POP) || (type_code == JMP)) {
		arg_type = AssemblyArg(str, &type_code, arg_count, &reg_num, &value);
		if (arg_type == ERROR) return ERROR;
	}
	CSInsert(arg_type, cs, &value, &reg_num, &type_code);
	return EMPTY;
}

int AssemblyProgram(CS* cs, FileInfo* file) {

	char* CS_ptr = cs->CS;
	for (int i = 0; i < file->n_lines; i++) {
		CSDump(cs);
		int n = AssemblyCommand(file->text[i], cs);
		if (n == ERROR) {
			fprintf(stderr, "Unknown command in %d line", i + 1);
			return ERROR;
		}
	}
	cs->CS = CS_ptr;
}