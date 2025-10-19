#define _CRT_SECURE_NO_WARNINGS
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

typedef struct {
	FILE* file;
	int initialized;
	LogLevel Level;

}Logger;

static Logger* getLogger() {
	static Logger logger = { NULL,0,LOG_INFO };

	if (!logger.initialized) {

		logger.file = fopen("app.log", "a");

		if (!logger.file) {
			fprintf(stderr, "Failed to open log file\n");
			exit(EXIT_FAILURE);
		}
		logger.initialized = 1;
		atexit(closeLogger);
	}
	return &logger;
}

void logMessage(LogLevel level, const char* format, ...)
{
	Logger* logger = getLogger();

	if (!logger->file) return;

	time_t now = time(NULL);
	struct tm* t = localtime(&now);

	const char* levelStr[] = { "INFO", "WARNING", "ERROR" };

	fprintf(logger->file, "[%02d:%02d:%02d] [%s] ",
		t->tm_hour, t->tm_min, t->tm_sec, levelStr[level]);

	va_list args;
	va_start(args, format);
	vfprintf(logger->file, format, args);
	va_end(args);

	fprintf(logger->file, "\n");
	fflush(logger->file);
}


void closeLogger(void)
{
	Logger* logger = getLogger();

	if (logger->file) {
		fclose(logger->file);
		logger->file = NULL;
	}

}
