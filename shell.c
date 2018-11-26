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
#include "execcom.h"

//colors
#define NRM  "\x1B[0m"
#define GRN  "\x1B[32m"
#define BLU  "\x1B[34m"

void printprompt() {
	char * name = getenv("USER");
	char curDir[1000];
	getcwd(curDir, sizeof(curDir));
	printf("%s%s:%s%s$ %s", GRN, name, BLU, curDir, NRM);
}

int main() {
  int save_stdout = dup(STDOUT_FILENO);
  int save_stdin = dup(STDIN_FILENO);
	while(1) {
		unsigned char * line = calloc(1000, sizeof(char));
		printprompt();
    //dup2(save_stdout, STDOUT_FILENO);
    //dup2(save_stdin, STDIN_FILENO);
    //fflush(stdin);
    //fflush(stdout);
    liveRead(line, 1000);

    char ** semiColons = parse_argsSemiColon(line);
		int i = 0;
		while(semiColons[i]) {
      execcom_redir(semiColons[i]);
      i++;
		}
    free(line);

	}

	return 0;
}
