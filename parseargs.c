#include <stdlib.h>
#include <string.h>
#include "parseargs.h"

char ** parse_argsSemiColon(char * line) {
	char ** arr = malloc(1000 * sizeof(char *));
	int i = 0;
	while(line) {
		arr[i] = strsep(&line, ";");
		i++;
	}

	return arr;
}

char ** parse_argsSpace(int * argc, char * line) {
	char ** arr = malloc(1000 * sizeof(char *));
	int i = 0;
	clean_str(line);
	while(line) {
		arr[i] = strsep(&line, " ");
		i++;
	}

	*argc = i;
	return arr;
}

void replace_multi_string(char *str) {
	char *dest = str; /* Destination to copy to */
	while(*str != '\0')
	{
		while(*str == ' ' && *(str + 1) == ' ')
			str++;
		*dest++ = *str++;
	}
	*dest = '\0';
}

void trim_whitespace(char *str) {
	char * cpy = (char *) malloc(strlen(str));
	strcpy(cpy, str);
	while((unsigned char) *cpy == ' ') {
		cpy++;
	}
	if(!*cpy) {
		strcpy(str, cpy);
		return;
	}
	char * e = cpy + strlen(cpy) - 1;
	while(e > cpy && (unsigned char) *e == ' ') {
		e--;
	}
	e[1] = 0;
	strcpy(str, cpy);
}

void clean_str(char *str) {
	replace_multi_string(str);
	trim_whitespace(str);
}


// char * hasPipe(char * line) {
//   char * p = strchr(line, '|');
//   return p;
// }
