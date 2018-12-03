#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include "parseargs.h"
#include "io.h"
#include "redirect.h"

//colors
#define NRM  "\x1B[0m"
#define GRN  "\x1B[32m"
#define BLU  "\x1B[34m"

char * niceDir(char * name) {
  char ** arr = calloc(1000, sizeof(char *));
  char cpy[1000];
  strcpy(cpy, name);
  int i = 0;
  while(name) {
    arr[i] = strsep(&name, "/");
    i++;
  }
  name = calloc(1000, sizeof(char));
  int j;
  for(j = 0; j < i; j++) {
    if(!strcmp(arr[j], getenv("USER"))) {
      break;
    }
  }
  if(j != i) {
    j++;
    strcpy(name, "~");
    for(; j < i; j++) {
      strcat(name, "/");
      strcat(name, arr[j]);
    }
  }
  else {
    strcpy(name, cpy);
  }
  free(arr);
  return name;
}

void printprompt() {
	char * name = getenv("USER");
	char curDir[1000];
	getcwd(curDir, 1000);
  char * dir = niceDir(curDir);
  strcpy(curDir, dir);
  free(dir);
	printf("%s%s:%s%s%s$ ", GRN, name, BLU, curDir, NRM);
}

int main() {
  int reg_stdout = dup(STDOUT_FILENO);
  int reg_stdin = dup(STDIN_FILENO);
  char origpath[1000];
  getcwd(origpath, 1000);
	while(1) {
		unsigned char * line = calloc(1000, sizeof(char));
		printprompt();
    liveRead(line, 1000, origpath);
    char ** semiColons = parse_argsSemiColon((char *)line);
		int i = 0;

		while(semiColons[i]) {
      handle_redirect(semiColons[i]);
      i++;
		}
    free(semiColons);
    free(line);
    dup2(reg_stdout, STDOUT_FILENO);
    dup2(reg_stdin, STDIN_FILENO);
	}

	return 0;
}
