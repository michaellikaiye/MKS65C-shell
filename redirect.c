#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "pipe.h"
#include "parseargs.h"

//Parse one full command, delineated by ; or \n, dealing with redirects
void handle_redirect(char * com) {
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
    } else if (!strcmp(argv[i], ">>")) {
      if(i + 1 >= argc) {
        printf("fysh: >>: output file required\n");
      } else {
        outf = open(argv[i+1], O_CREAT | O_WRONLY | O_APPEND, 0644);
        if(outf == -1) {
          printf("fysh: >>: %s\n", strerror(errno));
        }
        dup2(outf, STDOUT_FILENO);
        i++;
      }
    } else if (!strcmp(argv[i], "<<")) {
      if(i + 1 >= argc) {
        printf("fysh: <<: input file required\n");
      } else {
        inf = open(argv[i + 1], O_RDONLY);
        if(inf == -1) {
          printf("fysh: <<: %s\n", strerror(errno));
        }
        printf("<< yet to work\n");
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
  handle_pipes(nargc, cpy);

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
