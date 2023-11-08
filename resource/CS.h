#ifndef CS_DED
#define CS_DED
#include "RuzalLib/super_io.h"
#include "RuzalLib/logger_.h"
#include "utility.h"
#include "isa.h"

enum Errors_CS {
	CS_RANGE_ERROR = 0,
	CS_NULLPTR = 1
};

// code segment??
struct CS {
	char* CS;
	size_t capacity;
	size_t size;
	size_t ip;
	LogFile logfile;
};

int CSCtor(CS* cs, size_t size);
int CSDtor(CS* cs);
int CSDump(CS* cs);
int CSVerify(CS* cs);

int CSInsertFile(CS* cs, FileInfo* file);

#endif