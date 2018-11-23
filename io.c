#include <stdio.h>
#include <termios.h>
#include <unistd.h>

/* reads from keypress, doesn't echo */
int getch()
{
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

