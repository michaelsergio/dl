#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <sys/ioctl.h>

#define DEFAULT_COLUMN_LEN 80
#define DEFAULT_DASH "-"
#define DEFAULT_DASH_UNICODE "─"

#ifdef DEBUG
  #define DEBUG_LOG(...) printf(__VA_ARGS__)
#else
  #define DEBUG_LOG(...)
#endif

typedef struct {
  unsigned int columns;
  char *dash;
  size_t dash_len;
} line_options_t;

unsigned int get_column_width_from_term() {
  struct winsize winsz;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz);
  return (unsigned int) winsz.ws_col;
}


// TEST when i+1 is NULL.
// What if the strsize of LANG is too short.
    // Increment i only after a mbchar has been printed.
// I dont fully trust this code.
bool is_unicode_supported() {
  // The LANG enviorment variable should be defined with UTF in its name.
  char *lang = getenv("LANG");
  DEBUG_LOG("Current Env: %s\n", lang);

  for(unsigned int i = 0; lang[i + 2] != 0; i++) {
    if ((lang[i] == 'u' && lang[i + 1] == 't' && lang[i + 2] == 'f') ||
        (lang[i] == 'U' && lang[i + 1] == 'T' && lang[i + 2] == 'F')) 
    {
        return true;
    }
  }
  return false;
}

size_t put_mbchar(char *str_ring, size_t str_ring_len) {
    size_t i;
    for (i = 0; i < mblen(str_ring, str_ring_len); i++) {
      putchar(str_ring[i]);
    }
    return i;
}

void drawLine(line_options_t *options) {
  unsigned int i;
  char *str_ring = options->dash;
  size_t str_ring_len = strlen(options->dash);
  size_t position = 0;

  for (i = 0; i < options->columns; i++)  {
    position += put_mbchar(str_ring + position, str_ring_len);
    position %= str_ring_len; // Go back to begining of ring if out.
    // Increment i only after a mbchar has been printed.
  }
  putchar('\n');
  DEBUG_LOG("Printed %d characters\n", i);
}


int main(int argc, const char *argv[])
{
  // The default line options.
  line_options_t line_options = { 
    .columns=DEFAULT_COLUMN_LEN,
    .dash=(char *)DEFAULT_DASH,
    .dash_len=sizeof(DEFAULT_DASH) - 1 // Ignore NULL Char
  };

  // Sets the program's locale so functions like mblen function properly.
  setlocale(LC_CTYPE, "");

  line_options.columns = get_column_width_from_term();

  // Switches default to unicode default.
  bool unicode_supported = is_unicode_supported();
  if (unicode_supported) {
    line_options.dash = (char *) DEFAULT_DASH_UNICODE;
    line_options.dash_len = strlen(DEFAULT_DASH_UNICODE);
  }

  drawLine(&line_options);
  return 0;
}

