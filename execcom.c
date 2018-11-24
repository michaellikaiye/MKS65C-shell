#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>

#include "parseargs.h"
#include "execcom.h"

//Parse one full command, delineated by ; or \n, dealing with redirects
void execcom_redir(char * com) {
  int argc;
  char ** argv = parse_argsSpace(&argc, com);
  int reg_stdout = dup(STDOUT_FILENO);
  int reg_stdin = dup(STDIN_FILENO);
  int outf = -1;
  int inf = -1; //output + input files
  char ** cpy = malloc(argc * sizeof(char *));
  int nargc = 0;

  for(int i = 0; i < argc; i++) {
    if(!strcmp(argv[i], ">")) {
      if(i + 1 >= argc) {
        printf("fysh: >: output file required\n");
      } else {
        outf = open(argv[i+1], O_CREAT | O_WRONLY, 0644); 
        if(outf == -1) {
          printf("fysh: >: %s\n", strerror(errno));
        }
        dup2(outf, STDOUT_FILENO);
        i++;
      }
    } else if (!strcmp(argv[i], "<")) {
      if(i + 1 >= argc) {
        printf("fysh: <: input file required\n");
      } else {
        inf = open(argv[i + 1], O_RDONLY);
        if(inf == -1) {
          printf("fysh: <: %s\n", strerror(errno));
        }
        dup2(inf, STDIN_FILENO);
        i++;
      }
    } else {
      cpy[nargc] = argv[i];
      nargc++;
    }
  }
  //Execute command
  execcom_builtins(nargc, cpy);
  //cleanup
  free(cpy);
  fflush(stdout);
  fflush(stdin);
  if(outf != -1) {
    if(close(outf) == -1) {
      printf("fysh: %s\n", strerror(errno));
    }
  }
  if(inf != -1) {
    if(close(inf) == -1) {
      printf("fysh: %s\n", strerror(errno));
    }
  }
  dup2(reg_stdout, STDOUT_FILENO);
  dup2(reg_stdin, STDIN_FILENO);
  close(reg_stdout);
  close(reg_stdin);
}

//execute a command, after redirects and (TBA) pipes, dealing with builtin fysh funcs
void execcom_builtins(int argc, char ** argv) { 
  if(!strcmp(argv[0], "exit")) {
    exit(0);
  }
  else if(!strcmp(argv[0], "cd")) {
    if(argc > 3) {
      printf("fysh: cd takes 1 argument, %d found\n", argc - 1);
    }
    int stat = chdir(argv[1]);
    if(stat == -1) {
      printf("fysh: cd: %s\n", strerror(errno));
    }
  }
  else {
    run(argc, argv);
  } 
}


// Function where the system command is executed
void run(int argc, char** argv) {
	// Forking a child
	int pid = fork();
	if(pid == -1) {
		printf("fysh: failed to fork child\n");
	}
	else if(pid == 0) {
		if(!argv) { return;
		}
		if(execvp(argv[0], argv) < 0) {
			if(strcmp(argv[0], ""))
        printf("fysh: %s", strerror(errno));
			// else input is nothing so do nothing
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}


