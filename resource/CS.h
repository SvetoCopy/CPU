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

struct CodeSeg {
	char* CodeSeg_ptr;
	size_t capacity;
	size_t size;
	LogFile logfile;
};

int CodeSegCtor(CodeSeg* code_seg, size_t size);
int CodeSegDtor(CodeSeg* code_seg);
int CodeSegDump(CodeSeg* code_seg, int ip);
int CodeSegVerify(CodeSeg* code_seg);

int CodeSegInsertFile(CodeSeg* code_seg, FileInfo* file);

#endif