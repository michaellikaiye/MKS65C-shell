#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "io.h"

int main() {
  unsigned char hi[1000];
  liveRead(hi, 1000);
  printf("|%s|\n", hi);  

  return 0;
}
