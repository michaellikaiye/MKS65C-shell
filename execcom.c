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
  char ** cpy = calloc(1000, sizeof(char *));
  int nargc = 0;

  //Deal with redirect symbols
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
  //null terminate argv array
  cpy[nargc] = NULL;
 
  //Execute command
  execcom_piping(nargc, cpy);

  //cleanup
  free(cpy);
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
  
  fflush(stdout); 
  fflush(stdin);
  close(reg_stdout);
  close(reg_stdin);
}

void execcom_piping(int argc, char ** argv) { 
  //int clog = open("char.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
  
  //printf("%d", argc); 
  //write(clog, argv[0], sizeof(argv[0]) * sizeof(char));
  //char nl = '\n';
  //write(clog, &nl, sizeof(char));

  int cc = 0; //current command in comarr
  int ccargv = 0; //current arg in current command
  struct command * comarr = calloc(argc, sizeof(struct command));
  comarr[cc].argv = calloc(argc, sizeof(char **));

  for(int i = 0; i < argc; i++) {
    if(!strcmp(argv[i], "|")) {
      comarr[cc].argv[ccargv] = NULL;
      cc++;
      ccargv = 0;
      comarr[cc].argv = calloc(argc + 1, sizeof(char **));
    } else {
      comarr[cc].argv[ccargv] = argv[i];
      (comarr[cc].argc)++;
      ccargv++;
    }
  }
  comarr[cc].argv[ccargv] = NULL;
  //Input file descriptor, and array of pip file descriptors
  int infd, pfd[2];
  infd = STDIN_FILENO;
  
  int i;
  for(i = 0; i < cc; i++) {
    pipe(pfd);
    //give write end of pipe to new proc, and old in as input
    makeproc(infd, pfd[1], comarr + i);
    //no use for old write end when done
    close(pfd[1]);
    free(comarr[i].argv);
    free(comarr + i);
    //give new read end to next prog
    infd = pfd[0];
  }

  makeproc(infd, STDOUT_FILENO, comarr + i); 
  free(comarr);
}

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


