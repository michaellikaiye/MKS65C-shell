#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#include <pwd.h>

#include "parseargs.h"
#include "execcom.h"
//Make a process
int makeproc(int infd, int outfd, struct command *com) {
  //Child proc and handle builtins
  if(!strncmp(com->argv[0], "exit", 4)) {
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
    exit(0); //error!
    // else input is nothing so do nothing
  }
}

int cd(struct command *com) {
  struct passwd *pw = getpwuid(getuid());
  if(com->argc > 3) {
    printf("fysh: cd takes 1 argument, %d found\n", com->argc - 1);
  }
  if(com->argc == 1) {
    int stat = chdir(pw->pw_dir);
    if(stat == -1) {
      printf("fysh: cd: %s\n", strerror(errno));
      return 1;
    }
  } else {
    int stat = chdir(com->argv[1]);
    if(stat == -1) {
      printf("fysh: cd: %s\n", strerror(errno));
      return 1;
    }
  }
  return 0;
}
