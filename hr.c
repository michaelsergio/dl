#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

typedef struct {
  unsigned int columns;
} line_options_t;

unsigned int getColumnWidthFromOS() {
  struct winsize winsz;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz);
  return (unsigned int) winsz.ws_col;
}

void drawLine(line_options_t *options) {
  for (unsigned int i = 0; i < options->columns; i++)  {
    putchar('-');
  }
  putchar('\n');
}

int main(int argc, const char *argv[])
{
  line_options_t options = { 
    .columns=80 
  };

  options.columns = getColumnWidthFromOS();

  drawLine(&options);

  return 0;
}

