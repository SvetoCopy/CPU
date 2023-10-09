#include "disassembler.h"

#define DEF_CMD(name, code, ...) \
if (type_code == code){\
	strcpy(type_str, #name) ;\
	is_found = true;\
}

LogFile loggerDASM;

int DisassemblyCommand(char* str, char* res) {

	assert(str != 0);
	int type_code = -1;
	int value = 0;

	int arg_count = sscanf(str, "%d %d", &type_code, &value);
	if (arg_count <= 0) return -1;
	bool is_found = false;
	char type_str[COMMAND_SIZE] = {};

	// find command ( check define )
	#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
	if (is_found == false) return -1;
	if (type_code == PUSH) {
		int n = sprintf_s(res, COMMAND_SIZE, "%s %d", type_str, value);
		return 0;
	}

	int n = sprintf_s(res, COMMAND_SIZE, "%s", type_str);
	return 0;
}

int DisassemblyProgramm(FileInfo* file, const char* res_file) {
	FILE* res = {};
	LogFileCtor("ASM_log.log", &loggerDASM);

	fopen_s(&res, res_file, "w");

	assert(res != 0);

	for (int i = 0; i < file->n_lines; i++) {

		char command[COMMAND_SIZE] = {};

		if (DisassemblyCommand(file->text[i], command) == -1) {
			fprintf(loggerDASM.file, "Unknown command in %d line", i + 1);
			LogFileDtor(&loggerDASM);
			return 0;
		}

		fprintf(res, "%s\n", command);
	}
	fclose(res);
	return 0;
}