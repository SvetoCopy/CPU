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
	return 0;
}

int SPUDtor(SPU* spu) {
	StackDtor(&(spu->stack));
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
	SPU spu = {};
	SPUCtor(&spu);

	for (int i = 0; i < file->n_lines; i++) {
		Command command = {};
		if (ReadCommand(file->text[i], &command) == -1) {
			fprintf(stderr, "Error in %d command", i + 1);
			return -1;
		}
		int a1 = 0;
		int b1 = 0;
		float a2 = 0;
		float b2 = 0;
		
		int n = 0;
		SPUDump(&spu);
		switch (command.type)
		{
		#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
		default:
			fprintf(stderr, "Error in %d command", i + 1);
			return -1;
			break;
		}
	}
}