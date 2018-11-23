#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "io.h"

#define KEY_DEL 127
#define KEY_ESC 27

int main() {
  unsigned char ch;
  while((ch = (unsigned char) getch()) != EOF) {
    if(ch == KEY_DEL) {
      delchar();
    } else if (ch == KEY_ESC ) {
      unsigned char buf[3];
      buf[0] = ch;
      buf[1] = getch();
      buf[2] = getch();
      if(buf[1] == '[') {
        switch(buf[2]) {
          case 'A':
            printf("Up Arrow");
            break;
          case 'B':
            printf("Down Arrow");
            break;
          case 'C':
            printf("Left Arrow");
            break;
          case 'D':
            printf("Right Arrow");
            break;
        }
      }
    } else {
      write(STDIN_FILENO, &ch, sizeof(unsigned char));
      //printf("\nchar number: %d\n", ch);
    }
  }

  return 0;
}
