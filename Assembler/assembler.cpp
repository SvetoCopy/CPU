#include "assembler.h"

LogFile loggerASM;

int LabelCtor(Label* label, char* name, double address) {
	label->name = name;
	label->address = address;
	return 0;
}

int LabelDtor(Label* label) {
	label->address = POISON;
	label->name = nullptr;
	return 0;
}

int LabelDump(Label* label) {
	printf("\s: \f \n", label->name, label->address);
	return 0;
}

int LabelVerify(Label* label) {
	if (label == nullptr) fprintf(stderr, "Label is nullptr");
	if (label->name == nullptr) fprintf(stderr, "Label name is nullptr");
	if (label->address == POISON) fprintf(stderr, "Label address is unknown");
	return 0;
}

int AssemblerCtor(Assembler* ASM, CS* cs, FileInfo* file) {
	ASM->cs = cs;
	ASM->file = file;
	ASM->labels = (Label*)calloc(INITIAL_COUNT_LABELS, sizeof(double) + sizeof(char*));
	return 0;
}
int AssemblerDtor(Assembler* ASM) {
	for (int i = 0; i < ASM->labels_count; i++) {
		LabelDtor(&ASM->labels[i]);
	}
	FileInfoDtor(ASM->file);
	CSDtor(ASM->cs);
	return 0;
}
int AssemblerDump(Assembler* ASM) {
	for (int i = 0; i < ASM->labels_count; i++) {
		LabelDump(&ASM->labels[i]);
	}
	CSDump(ASM->cs);
	return 0;
}
int AssemblerVerify(Assembler* ASM) {
	for (int i = 0; i < ASM->labels_count; i++) {
		LabelVerify(&ASM->labels[i]);
	}
	CSVerify(ASM->cs);
	return 0;
}

int WriteByteCode(CS* cs, const char* filename) {
	FILE* res = {};
	fopen_s(&res, filename, "wb");
	fwrite(cs->CS, sizeof(char), cs->size, res);
	fclose(res);
	return 0;
}

int CSInsertIntCode(CS* cs, int* code) {
	*(int*)(cs->CS + cs->size) = *code;
	cs->size += sizeof(int);
	return 0;
}

int CSInsertDoubleCode(CS* cs, double* code) {
	*(double*)(cs->CS + cs->size) = *code;
	cs->size += sizeof(double);
	return 0;
}

int CSInsert(int arg_type, CS* cs, double* value, int* reg_num, int* command) {
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

	case LABEL:
		CSDump(cs);
		CSInsertDoubleCode(cs, value);
		CSDump(cs);
		break;
	}
	return 0;
}

int DetermineLabel(Assembler* ASM, char* str) {
	for (int i = 0; i < ASM->labels_count; i++) {
		if (strcmp(str, ASM->labels[i].name)) return ASM->labels[i].address;
	}
	return ERROR;
}

int DetermineReg(char* str) {
	if ((strlen(str) == REG_SIZE) && (str[0] = 'R') && (str[2] = 'X')) {
		return str[1] - 'A';
	}
	return ERROR;
}

int CreateLabel(Assembler* ASM, char* name, double address) {

	LabelCtor(&ASM->labels[ASM->labels_count], name, address);
	ASM->labels_count++;
	return 0;
}

// Split to some func
int AssemblyStr(Assembler* ASM, char* str, int* type_code, int* reg_num, double* value, size_t passage) {
	char type_str[COMMAND_SIZE] = {};
	char arg_name[COMMAND_SIZE] = {};

	int  arg_count2 = sscanf(str, "%s %s", &type_str, &arg_name);
	if (arg_count2 < 2) return ERROR;
	
	*reg_num = DetermineReg(arg_name);
	if (*reg_num != ERROR) {
		Opcode opcode = { *type_code, REG };
		*type_code = *(int*)(&opcode);

		*value = -1;
		return REG;
		
	}
	else {
		if (passage == 1) return LABEL;
		*value = DetermineLabel(ASM, arg_name);

		Opcode opcode = { *type_code, IMM };
		*type_code = *(int*)(&opcode);

		if (*value == ERROR) return ERROR;
		return LABEL;
	}
	
}

int AssemblyImm(char* str, int* type_code, double* value) {
	Opcode opcode = { *type_code, IMM };
	*type_code = *(int*)(&opcode);
	return IMM;
}

int AssemblyArg(Assembler* ASM, char* str, int* type_code, int arg_count, int* reg_num, double* value, size_t passage) {
	if (arg_count == 1) {
		return AssemblyStr(ASM, str, type_code, reg_num, value, passage);
	}
	else {
		return AssemblyImm(str, type_code, value);
	}
}

int AssemblyCommand(char* str, Assembler* ASM, size_t passage) {

	assert(str != 0);

	double value                  = 0;
	int    reg_num                = 0;
	int    type_code              = 0;
	int    arg_type               = EMPTY;
	char   type_str[COMMAND_SIZE] = {};

	// %[]
	// %n
	int arg_count = sscanf(str, "%s %lf", &type_str, &value);
	if (arg_count <= 0) return ERROR;
	int type_str_len = strlen(type_str);

	#define DEF_CMD(name, code, ...) else if (strcmp(type_str, #name) == 0){ type_code = code;}
	if (0) {}
	#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"
	#undef DEF_CMD
	else if (type_str[type_str_len - 1] == ':') {
		if (passage == 1) {
			type_str[type_str_len - 1] = '\0';
			CreateLabel(ASM, type_str, ASM->cs->size);
		}
		return 0;
	}
	else return ERROR;
	
	if ((type_code == PUSH) || (type_code == POP) || (type_code == JMP) 
		|| ((type_code >= JA) && (type_code <= JNE)) || (type_code == CALL)) {
		arg_type = AssemblyArg(ASM, str, &type_code, arg_count, &reg_num, &value, passage);
		if (arg_type == ERROR) return ERROR;
	}

	CSInsert(arg_type, ASM->cs, &value, &reg_num, &type_code);
	return 0;
}

int AssemblyProgram(Assembler* ASM){
	// first passage( for labels ) 
	for (int i = 0; i < ASM->file->n_lines; i++) {
		AssemblerDump(ASM);
		int n = AssemblyCommand(ASM->file->text[i], ASM, 1);
		if (n == ERROR) {
			fprintf(stderr, "Unknown command in %d line", i + 1);
			return ERROR;
		}
	}
	ASM->cs->size = 0;
	// second passage 
	for (int i = 0; i < ASM->file->n_lines; i++) {
		AssemblerDump(ASM);
		int n = AssemblyCommand(ASM->file->text[i], ASM, 2);
		if (n == ERROR) {
			fprintf(stderr, "Unknown command in %d line", i + 1);
			return ERROR;
		}
	}
	AssemblerDump(ASM);
}