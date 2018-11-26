struct command {
  int argc;
  char ** argv;
};

void execcom_piping(int argc, char ** argv);
int makeproc(int infd, int outfd, struct command *com);
void execprog(struct command *com);
int cd(struct command *com);

void execcom_redir(char * com);
void execcom_builtins(int argc, char ** argv);
void run(int argc, char** argv);

