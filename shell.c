/*errors
    when entering spaces where they don't belong, command takes space as input
      fix parse_args function
		big error when entering nothing into shell line
			fix main
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

char ** parse_argsSemiColon(char * line) {
	char ** arr = malloc(15 * sizeof(char *));
	int i = 0;
	while(line) {
		arr[i] = strsep(&line, ";");
		i++;
	}

	return arr;
}
char ** parse_args( char * line) {
	char ** arr = malloc(15 * sizeof(char *));
	int i = 0;
	while(line) {
		arr[i] = strsep(&line, " ");
		i++;
	}

	return arr;
}

// Function where the system command is executed
void run(char** args) {
	// Forking a child
	int pid = fork();

	if(pid == -1) {
		printf("failed to fork child\n");
		return;
	}
	else if(pid == 0) {
		if(execvp(args[0], args) < 0) {
			printf("failed to execute command\n");
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}
void printstart() {
	char curD[100];
	getcwd(curD, sizeof(curD));
	printf("\x1B[34m");
	printf("%s", curD);
	printf("$ ");
	printf("\x1B[0m");
}

int main() {
	//execvp(args[0], args);

	char line[100];

	while(1) {
		printstart();
		scanf("%[^\n]%*c", line);

		char ** semiColons = parse_argsSemiColon(line);
		int i = 0;
		while(semiColons[i]) {
			char ** args = parse_args(semiColons[i++]);
			if(strcmp(line, "") == 0)
				printf("to be worked on infinity\n");
			else if(strcmp(args[0], "exit") == 0) {
				printf("closing shell\n");
				exit(0);
			}
			else if(strcmp(args[0], "cd") == 0) {
				printf("to be worked on\n");
				//return 0; //maybe be something else?idk
			}
			else {
				run(args);
			}
		}
	}

	//char * str = args[0];
	//printdir(str);
	return 0;
}
