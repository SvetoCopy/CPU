#include "assembler.h"

#define DEF_CMD(name, code, ...) if (strcmp(type, #name) == 0) type_code = code;

Logger loggerASM;

int AssemblyCommand(char* str, char* res) {

	assert(str != 0);
	char type[COMMAND_SIZE];
	int value = 0;

	int arg_count = sscanf(str, "%s %d", &type, &value);

	if (arg_count <= 0) return -1;

	int type_code = -1;

	// find command ( check define )
#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
	if (type_code == -1) return -1;
	if (type_code == PUSH) {
		int n = sprintf_s(res, COMMAND_SIZE, "%d %d", type_code, value);
		return 0;
	}

	int n = sprintf_s(res, COMMAND_SIZE, "%d", type_code);
	return 0;
}
int AssemblyProgramm(FileInfo* file, const char* res_file) {
	FILE* res = {};
	LoggerCtor("ASM_log.log", &loggerASM);

	fopen_s(&res, res_file, "w");

	assert(res != 0);

	for (int i = 0; i < file->n_lines; i++) {

		char command[COMMAND_SIZE] = {};

		if (AssemblyCommand(file->text[i], command) == -1) {
			fprintf(loggerASM.file, "Unknown command in %d line", i + 1);
			LoggerDtor(&loggerASM);
			return 0;
		}

		fprintf(res, "%s\n", command);
	}
	fclose(res);
	return 0;
}