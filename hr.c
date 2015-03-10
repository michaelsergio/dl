#include <stdio.h>

void drawLine(unsigned int columns) {
  for (unsigned int i = 0; i < columns; i++)  {
    printf("-");
  }
  printf("\n");
}

int main(int argc, const char *argv[])
{
  unsigned int columns = 80;
  drawLine(columns);
  return 0;
}

