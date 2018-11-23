#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))
#define KEY_DEL 127
#define KEY_ESC 27
//replace write with printf or other way around? inconsistant 


/* reads from keypress, doesn't echo */
int getch() {
  struct termios old, new;
  int ch;
  tcgetattr( STDIN_FILENO, &old );
  new = old;
  //Unset canonical and echo flags, so we can handle input
  new.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &new );
  //read char off stdin
  ch = getchar();
  //Change back into old term mode
  tcsetattr( STDIN_FILENO, TCSANOW, &old );
  return ch;
}

void delchar() {
  char buf[] = "\b \b";
  write(STDIN_FILENO, buf, sizeof(buf) - 1);
}

int maxN(int a, int b) {
  if(a > b)
    return a;
  else
    return b;
}

void liveRead(unsigned char * buffer, int count) { //unsigned char ** oldbuf, int oldCount
  unsigned char ch;
  int counter = 0;
  int cMax = 0;
  while((ch = (unsigned char) getch()) != 10 && counter < count) {
    if(ch == KEY_DEL) {
      if(counter < cMax) {
	; //shift right side stuff or remove it when deleting
      }
      if(counter > 0) {
	delchar();
	counter--;
	cMax--;
      }
    }
    //stuff for tab completion
    else if (ch == KEY_ESC ) {
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
	  if(counter < cMax) {
	    counter++;
	    cursorforward(1);
	  }
	  //printf("Left Arrow");
	  break;
	case 'D':
	  if(counter > 0) {
	    counter--;
	    cursorbackward(1);
	  }
	  //printf("Right Arrow");
	  break;
        }
      }
    } else {
      //edit to shift text after arrow key movement
      cMax = maxN(cMax, counter);
      buffer[counter] = ch;
      counter++;
      write(STDIN_FILENO, &ch, sizeof(unsigned char));
      //printf("\nchar number: %d\n", ch);
    }
  }
  buffer[counter] = '\0';
  printf("\n");
}


