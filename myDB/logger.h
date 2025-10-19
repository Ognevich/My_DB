#ifndef _LOGGER_H_
#define _LOGGER_H_

typedef enum {
	LOG_INFO,
	LOG_WARINGNG,
	LOG_ERROR
}LogLevel;


void logMessage(LogLevel level, const char* format, ...);
void closeLogger(void);


#endif
