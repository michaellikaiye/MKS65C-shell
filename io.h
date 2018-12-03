#ifndef IO
#define IO

struct histline {
  int size;
  char * line;
};

unsigned char * getch();
void liveRead(unsigned char * line, int count, char * origpath);
#endif
