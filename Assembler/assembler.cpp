#include "assembler.h"

int AssemblerOutputFile(Assembler* ASM, const char* filename) {
	FILE* res = {};
	fopen_s(&res, filename, "wb");

	fwrite(ASM->code_seg, sizeof(char), ASM->code_seg_size, res);

	fclose(res);

	return 0;
}

int AssemblerInsertIntCode(Assembler* ASM, int code) {
	*(int*)(ASM->code_seg + ASM->code_seg_size) = code;
	ASM->code_seg_size += sizeof(int);

	return 0;
}

int AssemblerInsertDoubleCode(Assembler* ASM, double code) {
	*(double*)(ASM->code_seg + ASM->code_seg_size) = code;
	ASM->code_seg_size += sizeof(double);

	return 0;
}

int LabelCtor(Label* label, char* name, double address) {
	label->name    = _strdup(name);
	label->address = address;

	return 0;
}

int LabelDtor(Label* label) {
	free(label->name);
	label->address = POISON;
	label->name    = nullptr;

	return 0;
}

int LabelDump(Label* label) {
	printf("%s: %f \n", label->name, label->address);

	return 0;
}

int LabelVerify(Label* label) {
	if (label == nullptr)        { fprintf(stderr, "Label is nullptr"); return ERROR; };
	if (label->name    == nullptr) fprintf(stderr, "Label name is nullptr");
	if (label->address == POISON)  fprintf(stderr, "Label address is unknown");

	return 0;
}

int AssemblerCtor(Assembler* asm_ptr, char* CodeSeg, FileInfo* file) {
	asm_ptr->code_seg = (char*)calloc(file->buff_size, sizeof(double) + sizeof(char*));;
	asm_ptr->file    = file;
	asm_ptr->labels  = (Label*)calloc(INITIAL_COUNT_LABELS, sizeof(double) + sizeof(char*));

	return 0;
}

int AssemblerDtor(Assembler* asm_ptr) {
	for (int i = 0; i < asm_ptr->labels_count; i++) LabelDtor(&asm_ptr->labels[i]);

	FileInfoDtor(asm_ptr->file);
	free(asm_ptr->labels);
	free(asm_ptr->code_seg);

	asm_ptr->code_seg_size = 0;

	return 0;
}

int AssemblerDump(Assembler* asm_ptr) {
	for (int i = 0; i < asm_ptr->labels_count; i++) LabelDump(&asm_ptr->labels[i]);
	
	size_t iter = 0;
	char* cs_var = asm_ptr->code_seg;

	printf("\n------------------------\n");
	while (iter < asm_ptr->code_seg_size) {
		Opcode opcode = *(Opcode*)((int*)(cs_var));
		printf(" %d ", *(int*)(cs_var));

		cs_var = (char*)((int*)(cs_var)+1);

		iter += sizeof(int);

		if ((opcode.code == PUSH) || (opcode.code == POP) || (opcode.code == JMP)
			|| ((opcode.code >= JA) && (opcode.code <= JNE)) || (opcode.code == CALL)) {

			if (opcode.arg_type == REG) {
				printf(" %d ", *(int*)(cs_var));
				cs_var = (char*)((int*)cs_var + 1);
				iter += sizeof(int);
			}
			else if (opcode.arg_type == IMM) {
				printf(" %lf ", *(double*)(cs_var));
				cs_var = (char*)((double*)cs_var + 1);
				iter += sizeof(double);
			}
			else if (opcode.arg_type == LABEL) {
				printf(" %lf ", *(double*)(cs_var));
				cs_var = (char*)((double*)cs_var + 1);
				iter += sizeof(double);
			}
		}
	}
	printf("\n------------------------\n");

	return 0;
}

int AssemblerVerify(Assembler* asm_ptr) {
	for (int i = 0; i < asm_ptr->labels_count; i++) LabelVerify(&asm_ptr->labels[i]);

	return 0;
}

ArgType FindLabelAddr(Assembler* asm_ptr, char* str, double* address) {
	for (int i = 0; i < asm_ptr->labels_count; i++) {
		if (strcmp(str, asm_ptr->labels[i].name) == 0) {
			*address = asm_ptr->labels[i].address;

			return LABEL;
		}
	}

	return UNDEFINED;
}

ArgType DetermineReg(char* str, int* reg_id) {

	#define DEF_REG(reg_name, reg_code) \
	if (_stricmp(#reg_name, str) == 0){ \
		*reg_id = reg_code;             \
		return REG;                     \
	}
	#include "..\resource\def_reg.h"
	return UNDEFINED;
}

int CreateLabel(Assembler* asm_ptr, char* name, int address) {
	LabelCtor(&asm_ptr->labels[asm_ptr->labels_count], name, address);
	asm_ptr->labels_count++;

	return 0;
}

ArgType DetermineRAM(char* str, int* address_num) {
	char word_in_brackets[MAX_RAM_LEN];
	int arg_count = sscanf(str, "[%[^]]", &word_in_brackets);

	if (arg_count == 0) return UNDEFINED;

	int reg = 0;
	ArgType arg_type = DetermineReg(word_in_brackets, &reg);
	if (arg_type != UNDEFINED) {
		*address_num = reg;
		return RAM_REG;
	}

	int imm = atoi(word_in_brackets);
	if (imm != ERROR) {
		*address_num = imm;
		return RAM_IMM;
	}

	return UNDEFINED;
}

ArgType AssemblyRegCommand(Assembler* ASM, int command_code, int address_num) {
	Opcode opcode = { command_code, REG };
	command_code = *(int*)(&opcode);

	AssemblerInsertIntCode(ASM, command_code); 
	AssemblerInsertIntCode(ASM, address_num);

	return REG;
}

ArgType AssemblyRAM(Assembler* ASM, int command_code, int address_num, ArgType ram_arg_type) {
	Opcode opcode = { command_code, ram_arg_type };
	command_code = *(int*)(&opcode);

	AssemblerInsertIntCode(ASM, command_code);
	AssemblerInsertIntCode(ASM, address_num);

	return ram_arg_type;
}

ArgType AssemblyLabel(Assembler* ASM, int command_code, double address) {
	Opcode opcode = { command_code, IMM };
	command_code = *(int*)(&opcode);

	AssemblerInsertIntCode(ASM, command_code);
	AssemblerInsertDoubleCode(ASM, address);

	return LABEL;
}

ArgType AssemblyStrArg(Assembler* ASM, char* str, int command_code, size_t passage) {
	char type_str[COMMAND_SIZE] = {};
	char arg_text[COMMAND_SIZE] = {};

	int  read_count = sscanf(str, "%s %s", &type_str, &arg_text);
	if (read_count < 2) return UNDEFINED;

	int reg_num = 0;
	ArgType arg_type = DetermineReg(arg_text, &reg_num);
	if (arg_type != UNDEFINED) return AssemblyRegCommand(ASM, command_code, reg_num);

	int RAM_address = 0;
	ArgType ram_arg_type = DetermineRAM(arg_text, &RAM_address);
	if (ram_arg_type != UNDEFINED) return AssemblyRAM(ASM, command_code, RAM_address, ram_arg_type);
	
	double address_num = 0;
	arg_type = FindLabelAddr(ASM, arg_text, &address_num);
	if ((arg_type != UNDEFINED) || (passage == 1)) return AssemblyLabel(ASM, command_code, address_num);
	
	return UNDEFINED;
}

ArgType AssemblyImmArg(Assembler* ASM, char* str, int command_code) {
	char   type_str[COMMAND_SIZE] = {};
	double value                  = 0;

	int read_count = sscanf(str, "%s %lf", &type_str, &value);

	Opcode opcode = { command_code, IMM };
	command_code = *(int*)(&opcode);

	AssemblerInsertIntCode(ASM, command_code);
	AssemblerInsertDoubleCode(ASM, value);

	return IMM;
}

ArgType AssemblyArg(Assembler* ASM, char* str, int command_code, bool arg_is_imm, size_t passage) {
	if (arg_is_imm == false) return AssemblyStrArg(ASM, str, command_code, passage);

	return AssemblyImmArg(ASM, str, command_code);
}


void RemoveComments(char* str) {
	while (*str != '\0') {
		if (*str == ';') {
			*str = '\0';

			return;
		}
		str++;
	}
}

int VerifyCommand(char* str, int args_count) {
	char tmp1[COMMAND_SIZE] = {};
	char tmp2[COMMAND_SIZE] = {};
	int  read_size          = 0;

	if (args_count == 1) sscanf(str, "%s %s %n", &tmp1, &tmp2, &read_size);
	else                 sscanf(str, "%s %n", &tmp1, &read_size);

	while (str[read_size] != '\0') {
		if (str[read_size] != ' ') return ERROR;
		read_size++;
	}

	return 0;
}

ParseStatus ParseCommand(char* str, char* buffer, int* command_code) {
	if (0) {}
	#define DEF_CMD(cmd_name, cmd_code, cmd_args_num, ...)                 \
	else if (strcmp(buffer, #cmd_name) == 0){                              \
		*command_code = cmd_code;                                          \
		if (VerifyCommand(str, cmd_args_num) == ERROR) return PARSE_ERROR; \
		return FOUND;                                                      \
	}
	#include "..\resource\def_cmd.h"
	#undef DEF_CMD

	return CMD_UNFOUND;
}

ParseStatus ParseLabel(Assembler* asm_ptr, char* buffer, int passage) {
	int buffer_len = strlen(buffer);
	if (buffer[buffer_len - 1] == ':') {
		if (passage == 1) {
			buffer[buffer_len - 1] = '\0';
			CreateLabel(asm_ptr, buffer, asm_ptr->code_seg_size);
		}

		return FOUND;
	}

	return CMD_UNFOUND;
}

ArgType AssemblyCmdWithArgs(Assembler* ASM, char* str, int command_code, bool arg_is_imm, size_t passage) {
	ArgType arg_type = EMPTY;
	#define DEF_CMD(cmd_name, cmd_code, cmd_args_num, ...)                     \
	if ((command_code == cmd_code) && (cmd_args_num == 1)) {                   \
		arg_type = AssemblyArg(ASM, str, command_code, arg_is_imm, passage);   \
		if (arg_type == UNDEFINED) return UNDEFINED;                           \
		return arg_type;                                                       \
	}
	#include "../resource/def_cmd.h"
	#undef DEF_CMD()

	return EMPTY;
}

// NoArgs
void AssemblyCmdNoArgs(Assembler* ASM, int command_code) {
	AssemblerInsertIntCode(ASM, command_code);
}

int AssemblyCommand(char* str, Assembler* asm_ptr, size_t passage) {
	assert(str != 0);

	double  value                  = 0;
	int     address_num            = 0;
	int     command_code           = 0;
	char    buffer[COMMAND_SIZE]   = {};

	RemoveComments(str);
	// if ( isspace(*str) )
	// if ( *str == '' || *str == '\n' )
	if (*str == '\0' || *str == '\n') return 0;

	// Trying to read imm arg
	int arg_count = sscanf(str, "%s %lf", &buffer, &value);
	if (arg_count <= 0) return ERROR;
	

	bool arg_is_imm = (arg_count == 2);
	ParseStatus status = ParseCommand(str, buffer, &command_code);
	if (status == CMD_UNFOUND) {
		status = ParseLabel(asm_ptr, buffer, passage);

		if (status == FOUND) return 0;
		else return ERROR;
	}

	// Speculate that command with arg
	// if it is false then try to call AssemblyCmdNoArgs
	ArgType arg_type = AssemblyCmdWithArgs(asm_ptr, str, command_code, arg_is_imm, passage);
	if (arg_type == UNDEFINED) return ERROR;

	if (arg_type == EMPTY) AssemblyCmdNoArgs(asm_ptr, command_code);

	return 0;
}

int AssemblyAllCommands(Assembler* asm_ptr, int passage) {
	for (int i = 0; i < asm_ptr->file->n_lines; i++) {

		AssemblerDump(asm_ptr);
		int ret_value = AssemblyCommand(asm_ptr->file->text[i], asm_ptr, 1);

		if (ret_value == ERROR) {
			fprintf(stderr, "Unknown command in %d line", i + 1);

			return ERROR;
		}
	}
}

int AssemblyProgram(Assembler* asm_ptr) {

	AssemblyAllCommands(asm_ptr, 1);
	asm_ptr->code_seg_size = 0;
	AssemblyAllCommands(asm_ptr, 2);

	AssemblerDump(asm_ptr);
	return 0;
}