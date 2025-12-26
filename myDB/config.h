#ifndef _CONFIG_H
#define _CONFIG_H

#define FORBIDEN_SYMBOLS ",./';:][{}()!@#$%^&*-=+`~?|<>\"\\"

#define TABLE_NAME_SIZE 50
#define COLUMN_ARGUMENTS 2

#define SPECIAL_COMMANDS
//#undef SPECIAL_COMMANDS

#ifdef _WIN32
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#endif
