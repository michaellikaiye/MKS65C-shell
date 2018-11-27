#ifndef PARSEARGS
#define PARSEARGS

char ** parse_argsSemiColon( char * line );
char ** parse_argsPipe(char * line);
char ** parse_argsSpace( int * argc, char * line );
void clean_str(char *str);
#endif
