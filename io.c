#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define cursorforward(x) if(x > 0) printf("\033[%dC", (x))
#define cursorbackward(x) if(x > 0) printf("\033[%dD", (x))
#define KEY_DEL 127
#define KEY_ESC 27
#define KEY_ENT 10
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

int maxN(int a, int b) {
  if(a > b)
    return a;
  else
    return b;
}

void liveRead(unsigned char * line, int count) {
  unsigned char * ch = malloc(sizeof(unsigned char));
  int cursorpos = 0;
  int ncp = 0; //new cursorpos
  int size = 0;
  while((*ch = (unsigned char) getch()) != KEY_ENT && cursorpos < count) {
    if(*ch == KEY_DEL) {
      if(cursorpos > 0) {
        strcpy(line + cursorpos - 1, line + cursorpos);
        ncp--;
        size--;
      }       
    } else if (*ch == KEY_ESC) {
      unsigned char buf[3];
      buf[0] = *ch;
      buf[1] = getch();
      buf[2] = getch();
      if(buf[1] == '[') {
        switch(buf[2]) {
	        case 'A':
	          //printf("Up Arrow");
	          break;
	        case 'B':
	          //printf("Down Arrow");
	          break;
	        case 'C':
            if(cursorpos < size) {
              ncp++;
            }
	          break;
	        case 'D':
            if(cursorpos > 0) {
              ncp--;
            }
	          break;
          }
        }
      
    } else if (*ch >= 32 && *ch <= 126) {
      char * tmp = (char *) malloc(1000 * sizeof(char));
      strncpy(tmp, line, cursorpos);
      int len = strlen(tmp);
      strncat(tmp + len, ch, 1);
      len++;
      strcat(tmp + len, line + cursorpos);
      strcpy(line, tmp);
      ncp++;
      size++;
    }
    cursorbackward(cursorpos);
    line[size] = '\0';
    printf("%s ", line);
    cursorbackward(size + 1);
    cursorforward(ncp);
    cursorpos = ncp;
  }
  line[size] = '\0';
  printf("\n");
}

/* OLD LIVEREAD
void liveRead(unsigned char * buffer, int count) { //unsigned char ** oldbuf, int oldCount
  unsigned char ch;
  int counter = 0;
  int cMax = 0;
  while((ch = (unsigned char) getch()) != 10 && counter < count) {
    if(ch == KEY_DEL) {
      if(counter > 0) {
        if(counter >= cMax) {
          unsigned char buf[] = "\b \b";
          write(STDOUT_FILENO, buf, 3 * sizeof(unsigned char));
        } else {
        } 
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
	          if(counter <= cMax) {
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
*/



