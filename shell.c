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

//replace home dir with ~
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
	char * stat = getcwd(curDir, 1000);
  char * dir = niceDir(curDir);
  strcpy(curDir, dir);
  free(dir);
	printf("%s%s:%s%s%s$ ", GRN, name, BLU, curDir, NRM);
}

int main() {
  //save stdout for later
  int reg_stdout = dup(STDOUT_FILENO);
  //save stdin for later
  int reg_stdin = dup(STDIN_FILENO);
  char origpath[1000];
  char * stat = getcwd(origpath, 1000); //get original wd to store data in
	while(1) {
		unsigned char * line = calloc(1000, sizeof(char));
    //print term prompt (not dynamic :( )
		printprompt();
    //read line, delineated by ENTER
    liveRead(line, 1000, origpath);
    //replace tilde with home dir
    char * newLine = parse_comments((char *) line);
    char * homedirparsed = parse_argsHomeDir(newLine);
    //parse strings delineated by semicolons
    char ** semiColons = parse_argsSemiColon(homedirparsed);
		int i = 0;

		while(semiColons[i]) {
      //run commands, starting with redirects
      handle_redirect(semiColons[i]);
      i++;
		}

    //Clean up IO and free stuff
    free(homedirparsed);
    free(semiColons);
    free(line);
    dup2(reg_stdout, STDOUT_FILENO);
    dup2(reg_stdin, STDIN_FILENO);
	}

	return 0;
}
