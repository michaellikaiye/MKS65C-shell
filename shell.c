/*errors
   when entering spaces where they don't belong, command takes space as input
   fix parse_args function
   big error when entering nothing into shell line
   fix main
 */

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

//colors
#define NRM  "\x1B[0m"
#define GRN  "\x1B[32m"
#define BLU  "\x1B[34m"

// Function where the system command is executed
void run(char** args, int argc) {
	// Forking a child
	int pid = fork();
	if(pid == -1) {
		printf("fysh: failed to fork child\n");
	}
	else if(pid == 0) {
		if(!argc) { return;
		}
		if(execvp(args[0], args) < 0) {
			if(strcmp(args[0], ""))
				printf("fysh: command '%s' not found\n", args[0]);
			// else input is nothing so do nothing
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

void printprompt() {
	char * name = getenv("USER");
	char curDir[1000];
	getcwd(curDir, sizeof(curDir));
	printf("%s%s:%s%s$ %s", GRN, name, BLU, curDir, NRM);
}

int main() {
	//execvp(args[0], args);
	while(1) {
		unsigned char line[1000];
		printprompt();
		//scanf("%[^\n]%*c", line);
		liveRead(line, 1000);

		char ** semiColons = parse_argsSemiColon(line);
		int i = 0;
		while(semiColons[i]) {
			if(hasPipe(semiColons[i]) == NULL) {
				int argc;
				char ** argv = parse_argsSpace(&argc, semiColons[i]);
				if(strcmp(argv[0], "exit") == 0) {
					exit(0);
				} else if(strcmp(argv[0], "cd") == 0) {
					if(argc - 1 > 2) {
						printf("fysh: cd takes 1 argument, %d found\n", argc - 1);
						//change print!!
					}
					int stat = chdir(argv[1]);
					if(stat == -1) {
						printf("fysh: cd: %s\n", strerror(errno));
					}
				}
				else {
					run(argv, argc);
				}
			} else {
				printf("fysh: cannot pipe yet\n");     //do pipe stuff
			}
			i++;
		}
	}

	//char * str = args[0];
	//printdir(str);
	return 0;
}
