#ifndef PARSEARGS
#define PARSEARGS
#include "cleanhelp.h"

char ** parse_argsSemiColon( char * line );
char * hasPipe(char * line);
char ** parse_argsPipe(char * line);
char ** parse_argsSpace( int * argc, char * line );
void clean_str(char *str);
#endif
