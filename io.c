#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#include "io.h"

#define cursorforward(x) if(x > 0) printf("\033[%dC", (x))
#define cursorbackward(x) if(x > 0) printf("\033[%dD", (x))
#define KEY_DEL 127
#define KEY_ESC 27
#define KEY_ENT 10
#define KEY_TAB 9
//replace write with printf or other way around? inconsistant 


/* reads from keypress, doesn't echo */
unsigned char * getch() {
  struct termios old, new;
  unsigned char * ch = malloc(sizeof(char));
  tcgetattr( STDIN_FILENO, &old );
  new = old;
  //Unset canonical and echo flags, so we can handle input
  new.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &new );
  //read char off stdin
  *ch = (unsigned char) getchar();
  //Change back into old term mode
  tcsetattr( STDIN_FILENO, TCSANOW, &old );
  return ch;
}

void liveRead(unsigned char * line, int count) {
  int linf = open("fysh.lines", O_CREAT | O_RDWR | O_APPEND, 0644); //diff
  char history[1000][1000];
  int hc = 0;
  char * hisbuf = calloc(1000, sizeof(char));
  int r = read(linf, hisbuf, 1000);
  int bufc = 0;
  int chac = 0;
  int hislen = strlen(hisbuf);

  printf("%d",hislen);
  for(bufc; bufc < hislen; bufc++) {
    if(*(hisbuf + bufc) != '\n') {
      history[hc][chac] = *(hisbuf + bufc);
      chac++;
    } else {
      hc++;
      chac = 0;
    }
  }

  unsigned char * ch;
  int cursorpos = 0;
  int ncp = 0; //new cursorpos
  int size = 0;
  int hloc = hc;
  strcpy(history[hc], line);
  hc++;

  while(*(ch = getch()) != KEY_ENT && cursorpos < count) {
    if(*ch == KEY_DEL) {
      if(cursorpos > 0) {
        strcpy(line + cursorpos - 1, line + cursorpos);
        ncp--;
        size--;
      }       
    } else if (*ch == KEY_ESC) {
      unsigned char buf[3];
      buf[0] = *ch;
      buf[1] = *getch();
      buf[2] = *getch();
      if(buf[1] == '[') {
        switch(buf[2]) {
	        case 'A':
            printf("%s\n", history[--hloc]);
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
      
    } else if (*ch == KEY_TAB) {
      char * path = (char *) calloc(1000, sizeof(char));
      int bp = cursorpos;
      while(line[bp] != ' ' && bp) {
        bp--;
      }
      if(line[bp] == ' ') {
        bp++;
      }

      strncpy(path, line + bp, cursorpos - bp);
      if(strlen(path)) {
        //Open cwd
        DIR * d; 
        d = opendir(".");
        struct dirent *entry;
        char ** dirnames = calloc(5000, sizeof(char));
        int currdir = 0;
        while(entry = readdir(d)) {
          if(!strncmp(entry->d_name, path, strlen(path))) {
            dirnames[currdir] = calloc(1000, sizeof(char));
            strcpy(dirnames[currdir], entry->d_name);
            currdir++;
          }
        }
        closedir(d);
          
        if(currdir == 1) {
          if(strcmp(path, dirnames[currdir - 1])) {
            char * tmp = (char *) calloc(1000, sizeof(char));
            strncpy(tmp, line, bp);
            int len = strlen(tmp);
            strcat(tmp + len, line + cursorpos);
            strcpy(line, tmp);
            free(tmp);
            
          
            tmp = (char *) calloc(1000, sizeof(char));

            strncpy(tmp, line, cursorpos);
            len = strlen(tmp);
            strcat(tmp + len, dirnames[currdir - 1]);
            int dirlen = strlen(dirnames[currdir - 1]);
            len += dirlen; 
            strcat(tmp + len, line + cursorpos);
            len += strlen(line + cursorpos);
            strncpy(line, tmp, len);

            
            ncp += dirlen - strlen(path);
            
            size += dirlen - strlen(path); 
            free(tmp);
           }
        }
        
        for(int i = 0; i < currdir; i++) {
          free(dirnames[i]);
        }
        free(dirnames);
      }
      free(path);
    } else if (*ch >= 32 && *ch <= 126) {
      char * tmp = (char *) calloc(1000, sizeof(char));
      strncpy(tmp, line, cursorpos);
      int len = strlen(tmp); 
      strncat(tmp + len, ch, 1);
      len++;
      strcat(tmp + len, line + cursorpos);
      strcpy(line, tmp);
      ncp++;
      size++;
      free(tmp);
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

  write(linf, line, strlen(line) * sizeof(char));
  char nl = '\n';
  write(linf, &nl, sizeof(char));
}
