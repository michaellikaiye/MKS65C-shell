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
	char * tdir = calloc(1000, sizeof(char));
  if (strlen(name) > 5 && strncmp ("/home", name, 5) == 0) {
			char c;
			int sc = 0;
			c = '/';
			for( ; *name != '\0'; ++name) {
				if(*name == c)
					sc++;
				if(sc == 3) //skip 3 slashes... can be changed
					break;
			}

			strncpy (tdir + 1, name, 1000);
      tdir[0] = '~';
      return tdir;
    }
  else
    return name;
}


void printprompt() {
	char * name = getenv("USER");
	char curDir[1000];
	getcwd(curDir, sizeof(curDir));
	strcpy(curDir, niceDir(curDir));
	printf("%s%s:%s%s$ %s", GRN, name, BLU, curDir, NRM);

}

int main() {
  int reg_stdout = dup(STDOUT_FILENO);
  int reg_stdin = dup(STDIN_FILENO);
	while(1) {
		unsigned char * line = calloc(1000, sizeof(char));
		printprompt();
    liveRead(line, 1000);
    char ** semiColons = parse_argsSemiColon((char *)line);
		int i = 0;

		while(semiColons[i]) {
      handle_redirect(semiColons[i]);
      i++;
		}
    free(line);
    dup2(reg_stdout, STDOUT_FILENO);
    dup2(reg_stdin, STDIN_FILENO);
	}

	return 0;
}
