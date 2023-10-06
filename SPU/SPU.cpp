#include "SPU.h"

Logger loggerCPU;

int CPUDtor(CPU* cpu) {
	StackDtor(&(cpu->stack));
	return 0;
}

int ReadCommand(char* str, Command* command) {
	int count = sscanf_s(str, "%d %d", &(command->type), &(command->value));
	if ((count == 1) && (command->type != PUSH)) return 0;
	if ((count == 2) && (command->type == PUSH)) return 0;
	return -1;
}

#define DEF_CMD(name, code, handle) case name: handle; break;

int ExecuteProgramm(FileInfo* file, FILE* out) {
	CPU cpu = {};
	StackCtor(&(cpu.stack), file->n_lines);
	LoggerCtor("CPUlogger.log", &(loggerCPU));

	assert(loggerCPU.file != NULL);

	for (int i = 0; i < file->n_lines; i++) {
		Command command = {};
		if (ReadCommand(file->text[i], &command) == -1) {
			fprintf(loggerCPU.file, "Error in %d command", i + 1);
			LoggerDtor(&loggerCPU);
			return -1;
		}
		int a = 0;
		int b = 0;
		StackDump(&(cpu.stack));
		switch (command.type)
		{
		#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
		default:
			fprintf(loggerCPU.file, "Error in %d command", i + 1);
			LoggerDtor(&loggerCPU);
			return -1;
			break;
		}
	}
}