#include "assembler.h"

#define DEF_CMD(name, code, ...) if (strcmp(type, #name) == 0) type_code = code;

LogFile loggerASM;

int DetermineReg(char* str) {
	if ((strlen(str) == REG_SIZE) && (str[0] = 'R') && (str[2] = 'X')) {
		return str[1] - 'A';
	}
	else return -1;
}

int AssemblyCommand(char* str, FILE* res) {

	assert(str != 0);
	char type[COMMAND_SIZE];
	int  value = 0;

	// %[]
	// %n
	// 
	int arg_count = sscanf(str, "%s %d", &type, &value);

	if (arg_count <= 0) return -1;

	int type_code = -1;

	// find command ( check define )
#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
	if (type_code == -1) return -1;
	if (type_code == PUSH) {
		if (arg_count == 1) {
			type_code = 1 + pow(2, 6) * 2;

			
			char reg_name[1000] = {};
			
			int arg_count2 = sscanf(str, "%s %s", &type, &reg_name);

			// TODO: Write in logger
			if (arg_count2 < 2) return -1;
			int   reg_num = DetermineReg(reg_name);
			
			// TODO: Write in logger
			if (reg_num == -1) return -1;

			value = reg_num;

		}else type_code = 1 + pow(2, 6) * 1;

		int n = fprintf(res,"%d %d\n", type_code, value);
		return 0;
	}

	int n = fprintf(res, "%d\n", type_code);
	return 0;
}

int AssemblyProgramm(FileInfo* file, const char* res_file) {
	FILE* res = {};
	LogFileCtor("ASM_log.log", &loggerASM);

	fopen_s(&res, res_file, "w");

	assert(res != 0);

	for (int i = 0; i < file->n_lines; i++) {

		char command[COMMAND_SIZE] = {};

		if (AssemblyCommand(file->text[i], res) == -1) {
			fprintf(loggerASM.file, "Unknown command in %d line", i + 1);
			LogFileDtor(&loggerASM);
			return 0;
		}
	}
	fclose(res);
	return 0;
}