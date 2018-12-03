#ifndef EXECCOM
#define EXECCOM
struct command {
  int argc;
  char ** argv;
};

void execcom_piping(int argc, char ** argv);
int makeproc(int infd, int outfd, struct command *com);
void execprog(struct command *com);
int cd(struct command *com);
#endif
