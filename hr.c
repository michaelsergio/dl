#include <stdio.h>

typedef struct {
  unsigned int columns;
} line_options_t;

void drawLine(unsigned int columns) {
  for (unsigned int i = 0; i < columns; i++)  {
    printf("-");
  }
  printf("\n");
}

int main(int argc, const char *argv[])
{
  line_options_t options = { 
    .columns=80 
  };

  drawLine(options.columns);
  return 0;
}

