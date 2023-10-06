#ifndef DED_LOGGER
#define DED_LOGGER
#include "stdio.h"

struct Logger {
	FILE* file;
	const char* name;
	bool is_opened;
};

int LoggerCtor(const char* name, Logger* logger);
int LoggerDtor(Logger* logger);


#endif // !DED_LOGGER
