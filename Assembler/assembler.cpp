#include "assembler.h"


int LabelCtor(Label* label, char* name, double address) {
	label->name    = name;
	label->address = address;
	return 0;
}

int LabelDtor(Label* label) {
	label->address = POISON;
	label->name    = nullptr;
	return 0;
}

int LabelDump(Label* label) {
	printf("%s: %f \n", label->name, label->address);
	return 0;
}

int LabelVerify(Label* label) {
	if (label == nullptr) fprintf(stderr, "Label is nullptr");
	if (label->name == nullptr) fprintf(stderr, "Label name is nullptr");
	if (label->address == POISON) fprintf(stderr, "Label address is unknown");
	return 0;
}

int AssemblerCtor(Assembler* asm_ptr, CS* cs, FileInfo* file) {
	asm_ptr->cs     = cs;
	asm_ptr->file   = file;
	asm_ptr->labels = (Label*)calloc(INITIAL_COUNT_LABELS, sizeof(double) + sizeof(char*));
	return 0;
}
int AssemblerDtor(Assembler* asm_ptr) {
	for (int i = 0; i < asm_ptr->labels_count; i++) {
		LabelDtor(&asm_ptr->labels[i]);
	}
	FileInfoDtor(asm_ptr->file);
	CSDtor(asm_ptr->cs);
	return 0;
}
int AssemblerDump(Assembler* asm_ptr) {
	for (int i = 0; i < asm_ptr->labels_count; i++) {
		LabelDump(&asm_ptr->labels[i]);
	}
	CSDump(asm_ptr->cs);
	return 0;
}
int AssemblerVerify(Assembler* asm_ptr) {
	for (int i = 0; i < asm_ptr->labels_count; i++) {
		LabelVerify(&asm_ptr->labels[i]);
	}
	CSVerify(asm_ptr->cs);
	return 0;
}

int DetermineLabel(Assembler* asm_ptr, char* str) {
	for (int i = 0; i < asm_ptr->labels_count; i++) {
		if (strcmp(str, asm_ptr->labels[i].name) == 0) return asm_ptr->labels[i].address;
	}
	return ERROR;
}

static char* ToUpperWord(const char* str) {
	char* str_res = _strdup(str);
	for (int i = 0; str[i] != '\0'; i++) {
		str_res[i] = toupper(str[i]); 
	}
	return str_res;
}

int DetermineReg(char* str) {

#	define DEF_REG(reg_name, reg_code) if (strcmp(ToUpperWord(#reg_name), str) == 0) return reg_code;
	#include "..\resource\def_reg.h"
	else return ERROR;
}

int CreateLabel(Assembler* asm_ptr, char* name, double address) {
	char* s = _strdup(name);
	LabelCtor(&asm_ptr->labels[asm_ptr->labels_count], s, address);
	asm_ptr->labels_count++;
	return 0;
}

static int StringToInt(char* str) {
	int res = 0;
	int len = strlen(str);
	while (*str != '\0') {
		int ASCII_ZERO = 48;
		int ascii_num  = *str - ASCII_ZERO;
		if (ascii_num < 10) {
			res += ascii_num * (pow(10, len - 1));
			len--;
			str++;
		}
		else return ERROR;
	}
	return res;
}

ArgType DetermineRAM(char* str, int* address_num) {
	char word_in_brackets[MAX_RAM_LEN];
	int arg_count = sscanf(str, "[%[^]]", &word_in_brackets);
	if (arg_count == 0) return UNDEFINED;

	int reg = DetermineReg(word_in_brackets);
	if (reg != ERROR) {
		*address_num = reg;
		return RAM_REG;
	}

	int imm = StringToInt(word_in_brackets);
	if (imm != ERROR) {
		*address_num = imm;
		return RAM_IMM;
	}

	return UNDEFINED;
}

ArgType AssemblyReg(int* type_code, double* value) {
	Opcode opcode = { *type_code, REG };
	*type_code = *(int*)(&opcode);
	*value = -1;
	return REG;
}

ArgType AssemblyRAM(int* type_code, double* value, ArgType ram_arg_type) {
	Opcode opcode = { *type_code, ram_arg_type };
	*type_code = *(int*)(&opcode);
	*value = -1;
	return ram_arg_type;
}
ArgType AssemblyLabel(int* type_code, int* address_num) {
	
	Opcode opcode = { *type_code, IMM };
	*type_code = *(int*)(&opcode);
	*address_num = -1;
	return LABEL;
}

ArgType AssemblyStr(Assembler* asm_ptr, char* str, int* type_code, int* address_num, double* value, size_t passage) {
	char type_str[COMMAND_SIZE] = {};
	char arg_text[COMMAND_SIZE] = {};

	int  arg_count2 = sscanf(str, "%s %s", &type_str, &arg_text);
	if (arg_count2 < 2) return UNDEFINED;
	
	*address_num = DetermineReg(arg_text);
	if (*address_num != ERROR) {
		return AssemblyReg(type_code, value);
	}

	ArgType ram_arg_type = DetermineRAM(arg_text, address_num);
	if (ram_arg_type != UNDEFINED) {
		return AssemblyRAM(type_code, value, ram_arg_type);
	}

	*value = DetermineLabel(asm_ptr, arg_text);
	if (passage == 1) return LABEL;
	if (*value != ERROR){
		return AssemblyLabel(type_code, address_num);
	}
	return UNDEFINED;
}

ArgType AssemblyImm(char* str, int* type_code, double* value) {
	Opcode opcode = { *type_code, IMM };
	*type_code = *(int*)(&opcode);
	return IMM;
}

ArgType AssemblyArg(Assembler* asm_ptr, char* str, int* type_code, int arg_count, int* address_num, double* value, size_t passage) {
	if (arg_count == 1) {
		return AssemblyStr(asm_ptr, str, type_code, address_num, value, passage);
	}
	else {
		return AssemblyImm(str, type_code, value);
	}
}

int AssemblyCommand(char* str, Assembler* asm_ptr, size_t passage) {

	assert(str != 0);

	double  value                  = 0;
	int     address_num            = 0;
	int     type_code              = 0;
	ArgType arg_type               = EMPTY;
	char    type_str[COMMAND_SIZE] = {};

	// %[]
	// %n
	int arg_count = sscanf(str, "%s %lf", &type_str, &value);
	if (arg_count <= 0) return ERROR;
	int type_str_len = strlen(type_str);

	#define DEF_CMD(name, code, ...) else if (strcmp(type_str, #name) == 0){ type_code = code;}
	if (0) {}
	#include "..\resource\def_cmd.h"
	#undef DEF_CMD
	else if (type_str[type_str_len - 1] == ':') {
		if (passage == 1) {
			type_str[type_str_len - 1] = '\0';
			CreateLabel(asm_ptr, type_str, asm_ptr->cs->size);
		}
		return 0;
	}
	else return ERROR;
	
	if ((type_code == PUSH) || (type_code == POP) || (type_code == JMP)|| ((type_code >= JA) && (type_code <= JNE)) 
	|| (type_code == CALL)) {

		arg_type = AssemblyArg(asm_ptr, str, &type_code, arg_count, &address_num, &value, passage);
		if (arg_type == UNDEFINED) return ERROR;
	}

	CSInsert(arg_type, asm_ptr->cs, &value, &address_num, &type_code);
	return 0;
}

int AssemblyProgram(Assembler* asm_ptr){
	// first passage( for labels ) 
	for (int i = 0; i < asm_ptr->file->n_lines; i++) {
		AssemblerDump(asm_ptr);
		if (strcmp(asm_ptr->file->text[i], "") == 0 || strcmp(asm_ptr->file->text[i], "\n") == 0 ) continue;
		int n = AssemblyCommand(asm_ptr->file->text[i], asm_ptr, 1);
		if (n == ERROR) {
			fprintf(stderr, "Unknown command in %d line", i + 1);
			return ERROR;
		}
	}
	asm_ptr->cs->size = 0;
	// second passage 
	for (int i = 0; i < asm_ptr->file->n_lines; i++) {
		AssemblerDump(asm_ptr);
		if (strcmp(asm_ptr->file->text[i], "") == 0 || strcmp(asm_ptr->file->text[i], "\n") == 0) continue;
		int n = AssemblyCommand(asm_ptr->file->text[i], asm_ptr, 2);
		if (n == ERROR) {
			fprintf(stderr, "Unknown command in %d line", i + 1);
			return ERROR;
		}
	}
	AssemblerDump(asm_ptr);
}