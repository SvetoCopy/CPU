#include "CS.h"

static int SetError(unsigned* all_errors, int error) {
	*all_errors |= (1 << error);
	return 0;
}

static int CSPrintErrorInfo(unsigned error, CS* cs) {
#define check(error_code) ( error & (1 << error_code) ) == (1 << error_code)
	FILE* f = (cs->logfile).file;
	if (check(CS_RANGE_ERROR)) fprintf(f, "\nip > CS size\n");
	if (check(CS_NULLPTR))  fprintf(f, "\nCS is null\n");
#undef check
	return 0;
};


int CSCtor(CS* cs, size_t capacity) {
	cs->CS = (char*)calloc(capacity, sizeof(char));
	cs->capacity = capacity;
	cs->size = 0;
	cs->ip = 0;
	return 0;
}

int CSDtor(CS* cs) {
	free(cs->CS);
	cs->ip = 0;
	cs->capacity = 0;
	return 0;
}

int CSDump(CS* cs) {
	size_t iter = 0;
	char* cs_var = cs->CS;

	printf("\n------------------------\n");
	while (iter < cs->capacity) {
		Opcode opcode = *(Opcode*)((int*)(cs_var));

		if (iter == cs->ip) printf(" [%d] ", *(int*)(cs_var));
		else printf(" %d ", *(int*)(cs_var));

		cs_var = (char*)((int*)(cs_var)+1);

		iter += sizeof(int);

		if ((opcode.code == PUSH) || (opcode.code == POP)) {

			if (opcode.arg_type == 2) {

				if (iter == cs->ip) printf(" [%d] ", *(int*)(cs_var));
				else printf(" %d ", *(int*)(cs_var));
				cs_var = (char*)((int*)cs_var + 1);
				iter += sizeof(int);
			}
			else {

				if (iter == cs->ip) printf(" [%lf] ", *(double*)(cs_var));
				else printf(" %lf ", *(double*)(cs_var));
				cs_var = (char*)((double*)cs_var + 1);
				iter += sizeof(double);
			}
		}
	}
	printf("\n------------------------\n");
	return 0;
}

int CSVerify(CS* cs) {
	unsigned error = 0;
	if (cs->ip > cs->capacity) {
		SetError(&error, CS_RANGE_ERROR);
		fprintf(stderr, "RANGE ERROR");
	}

	if (cs->CS == nullptr) {
		SetError(&error, CS_NULLPTR);
		fprintf(stderr, "CS_NULLPTR");
	}
	if (error != 0) {
		CSPrintErrorInfo(error, cs);
		return ERROR;
	}
	return 0;
}

