#include "CS.h"


int CodeSegInsertFile(CodeSeg* code_seg, FileInfo* file) {
	fread(code_seg->CodeSeg_ptr, sizeof(char), file->buff_size, file->input_file);
	return 0;
}

static int SetError(unsigned* all_errors, int error) {
	*all_errors |= (1 << error);
	return 0;
}

static int CodeSegPrintErrorInfo(unsigned error, CodeSeg* code_seg) {
#define check(error_code) ( error & (1 << error_code) ) == (1 << error_code)
	FILE* file = code_seg->logfile.file;
	if (check(CS_RANGE_ERROR)) fprintf(file, "\nip > CS size\n");
	if (check(CS_NULLPTR))  fprintf(file, "\nCS is null\n");
#undef check
	return 0;
};


int CodeSegCtor(CodeSeg* code_seg, size_t capacity) {
	code_seg->CodeSeg_ptr = (char*)calloc(capacity, sizeof(char));
	code_seg->capacity = capacity;
	code_seg->size = 0;
	return 0;
}


int CodeSegDtor(CodeSeg* code_seg) {
	free(code_seg->CodeSeg_ptr);
	code_seg->capacity = 0;
	return 0;
}

int CodeSegDump(CodeSeg* code_seg, int ip) {
	size_t iter = 0;
	char* cs_var = code_seg->CodeSeg_ptr;

	printf("\n------------------------\n");
	while (iter < code_seg->capacity) {
		Opcode opcode = *(Opcode*)((int*)(cs_var));

		if (iter == ip) printf(" [%d] ", *(int*)(cs_var));
		else printf(" %d ", *(int*)(cs_var));

		cs_var = (char*)((int*)(cs_var)+1);

		iter += sizeof(int);

		if ((opcode.code == PUSH) || (opcode.code == POP) || (opcode.code == JMP) 
			|| ((opcode.code >= JA) && (opcode.code <= JNE)) || (opcode.code == CALL)) {

			if (opcode.arg_type == REG) {

				if (iter == ip) printf(" [%d] ", *(int*)(cs_var));
				else printf(" %d ", *(int*)(cs_var));
				cs_var = (char*)((int*)cs_var + 1);
				iter += sizeof(int);
			}
			else if (opcode.arg_type == IMM) {

				if (iter == ip) printf(" [%lf] ", *(double*)(cs_var));
				else printf(" %lf ", *(double*)(cs_var));
				cs_var = (char*)((double*)cs_var + 1);
				iter += sizeof(double);
			}
			else if (opcode.arg_type == LABEL) {

				if (iter == ip) printf(" [%lf] ", *(double*)(cs_var));
				else printf(" %lf ", *(double*)(cs_var));
				cs_var = (char*)((double*)cs_var + 1);
				iter += sizeof(double);
			}
		}
	}
	printf("\n------------------------\n");
	return 0;
}

int CodeSegVerify(CodeSeg* code_seg) {
	unsigned error = 0;
	if (code_seg->CodeSeg_ptr == nullptr) {
		SetError(&error, CS_NULLPTR);
		fprintf(stderr, "CS_NULLPTR");
	}
	if (error != 0) {
		CodeSegPrintErrorInfo(error, code_seg);
		return ERROR;
	}
	return 0;
}


