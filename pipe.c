#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>

#include "pipe.h"
#include "execcom.h"

void handle_pipes(int argc, char ** argv) {
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
    //give new read end to next prog
    infd = pfd[0];
  }

  makeproc(infd, STDOUT_FILENO, comarr + i);
  free(comarr);
}
