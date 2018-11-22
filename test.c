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

int main() {
  int ch;
  while((ch = (char) getch()) != EOF) {
    printf("Character: %c\n", ch);
    printf("Char Num:  %d\n", ch);
  }

  return 0;
}
