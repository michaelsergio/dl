#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEFAULT_COLUMN_LEN 80
#define DEFAULT_DASH "-"

typedef struct {
  unsigned int columns;
  char *dash;
  size_t dash_len;
} line_options_t;

unsigned int get_column_width_from_os() {
  struct winsize winsz;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz);
  return (unsigned int) winsz.ws_col;
}

void drawLine(line_options_t *options) {
  for (unsigned int i = 0; i < options->columns; i++)  {
      size_t j = i % (options->dash_len - 1); //ignore NULL in ring.
      putchar(options->dash[j]);
  }
  putchar('\n');
}

int main(int argc, const char *argv[])
{
  line_options_t options = { 
    .columns=DEFAULT_COLUMN_LEN,
    .dash=DEFAULT_DASH,
    .dash_len=sizeof(DEFAULT_DASH)
  };

  options.columns = get_column_width_from_os();
  printf("Doing %d times\n", options.columns);

  drawLine(&options);

  return 0;
}

