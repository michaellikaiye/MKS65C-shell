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
int makeproc(int infd, int outfd, struct command *com) { 
  //Child proc and handle builtins
  if(!strcmp(com->argv[0], "exit")) {
    exit(0);
  } 
  else if(!strcmp(com->argv[0], "cd")) {
    cd(com);
  } else { 
    pid_t cpid;
    cpid = fork();
    if(cpid == -1) {
      printf("fysh: failed to fork child");
    }
    else if(!cpid) {
      if(infd != STDIN_FILENO) {
        dup2(infd, STDIN_FILENO);
        close(infd);
      }
      if(outfd != STDOUT_FILENO) {
        dup2(outfd, STDOUT_FILENO);
        close(outfd);
      }
      execprog(com);
    } else {
      wait(NULL);
      return cpid;
    }
  }
}

void execprog(struct command *com) {
  if(execvp(com->argv[0], com->argv) == -1) {
    if(strcmp(com->argv[0], ""))
      printf("fysh: %s: %s\n",com->argv[0], strerror(errno));
    // else input is nothing so do nothing
  } 
}

int cd(struct command *com) {
  if(com->argc > 3) {
    printf("fysh: cd takes 1 argument, %d found\n", com->argc - 1);
  }
  int stat = chdir(com->argv[1]);
  if(stat == -1) {
    printf("fysh: cd: %s\n", strerror(errno));
    return 1;
  }
  return 0;
}


