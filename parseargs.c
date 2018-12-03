#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parseargs.h"
#include <pwd.h>

char * parse_comments(char * line) {
	char newLine[1000];
	strcpy(newLine, line);
	char * loc = strchr(newLine, '#');
	if(loc != NULL && *(loc - 1) == ' ')
		*(loc) = '\0';
	char * re = calloc(1, sizeof(char *));
	strcpy(re, newLine);
	return re;
}

//Parse line into semicolon delineated tokens
char ** parse_argsSemiColon(char * line) {
	char ** arr = calloc(1000, sizeof(char *));
	int i = 0;
	while(line) {
		arr[i] = strsep(&line, ";");
		i++;
	}

	return arr;
}

//replace ~ with homedir
char * parse_argsHomeDir(char * line) {
  char * arr = calloc(1000, sizeof(char *));
  int i;
  struct passwd *pw = getpwuid(getuid());
  while(line) {
    strcat(arr, strsep(&line, "~"));
    if(line) {
      strcat(arr, pw->pw_dir);
    }
  }
  return arr;
}

//Parse line into space delineated lexemes
char ** parse_argsSpace(int * argc, char * line) {
	char ** arr = calloc(1000, sizeof(char *));
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
	char * cpy = (char *) calloc(1, strlen(str));
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

//clean whitespace from string
void clean_str(char *str) {
	replace_multi_string(str);
	trim_whitespace(str);
}
