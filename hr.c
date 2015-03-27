#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include "ansi.h"

#define DEFAULT_COLUMN_LEN 80
#define DEFAULT_DASH "-"
#define DEFAULT_DASH_UNICODE "─"

#define PROGRAM_NAME "hr"

#ifdef DEBUG
  #define DEBUG_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
  #define DEBUG_LOG(...)
#endif

typedef struct {
  unsigned columns;
  char *dash;
  size_t dash_len;
  char *color;           // A string defined in ansi.h or NULL.
  const char *text;
} line_options_t;

typedef struct {
  char color;
  unsigned force_columns;
  char *force_dash;
  const char *text;
} cmd_options_t;

unsigned int get_column_width_from_term() {
  struct winsize winsz;
  int err = ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz);

  if (err) {
    DEBUG_LOG("Error determining window size. Using default size.\n");
    return DEFAULT_COLUMN_LEN;
  }

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

  if (options->color) {
    printf("%s", options->color);
  }

  for (i = 0; i < options->columns; i++)  {
    position += put_mbchar(str_ring + position, str_ring_len);
    position %= str_ring_len; // Go back to begining of ring if out.
    // Increment i only after a mbchar has been printed.
  }

  if (options->color) {
    printf("%s", ANSI_COLOR_RESET);
  }
  putchar('\n');

  DEBUG_LOG("Printed %d characters\n", i);
}

void show_usage() {
  fprintf(stderr, "Usage:  %s [flags] [text]\n", PROGRAM_NAME);
  fprintf(stderr, 
          "  -h  --help\n"
          "  -v  --version\n"
          "  -r  --red\n"
          "  -g  --green\n"
          "  -b  --blue\n"
          "  -y  --yellow\n"
          "  -c  --cyan\n"
          "  -m  --magenta\n"
          "  -k  --black\n"
          "  -w  --white\n"
          "  -d  --dash dash_symbol\n"
          "  -n  --columns number\n"
  );
}

void show_version() {
  fprintf(stderr, "hr version 0.0.1\n");
}

// TODO Remove this 
static int verbose_flag;
void check_options(int argc, const char **argv, cmd_options_t *cmd_options) {
  while (1) {
    static struct option long_options[] = {
      /* These options set a flag. */
      {"verbose", no_argument,       &verbose_flag, 1},
      {"brief",   no_argument,       &verbose_flag, 0},
      /* These options don’t set a flag.
         We distinguish them by their indices. */
      {"help",     no_argument,       0, 'h'},
      {"version",  no_argument,       0, 'v'},
      {"dash",     no_argument,       0, 'd'},
      {"columns",  required_argument, 0, 'n'},
      {"nounicode",no_argument,       0, 'u'},
      {"bs",       required_argument, 0, 'p'}, // pre-
      {"as",       required_argument, 0, 'a'}, // after-
      {"surround", required_argument, 0, 's'},
      {"red",      no_argument, 0, 'r'},
      {"green",    no_argument, 0, 'g'},
      {"blue",     no_argument, 0, 'b'},
      {"cyan",     no_argument, 0, 'c'},
      {"magenta",  no_argument, 0, 'm'},
      {"yellow",   no_argument, 0, 'y'},
      {"black",    no_argument, 0, 'k'},
      {"white",    no_argument, 0, 'w'},
      {0, 0, 0, 0}
    };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    int c = getopt_long(argc, argv, "rgbymckwn:a:p:d:vh", long_options, &option_index); 
    /* Detect the end of the options. */
    if (c == -1) {
      // If there is more after the options, use that as the input text.
      if (optind != argc) cmd_options->text = argv[optind];
      break;
    }

    switch(c) {
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0) break;
        printf ("option %s", long_options[option_index].name);
        if (optarg) printf (" with arg %s\n", optarg);
        break;

      case 'a':
        puts("option -a\n");
        break;

      case 'p':
        puts("option -p\n");
        break;

      case 'f':
        printf("option -f with value `%s'\n", optarg);
        break;

      case 'd':
        // TODO: Do I need to memcpy the item in optarg so I can use it later?
        // Or can I use the address since its created with the program args 
        // and will always exist?
        cmd_options->force_dash = optarg;
        break;

      case 'n': 
        //if (long_options[option_index].flag != 0) break;
        DEBUG_LOG("optarg: %s\n", optarg);
        cmd_options->force_columns = atoi(optarg);
        break;

      case 'v':
        show_version();
        exit(0);
        break;

      case '?': // Fallthorugh to help when error occurs.
      case 'h':
        show_usage();
        exit(0);
        break;

      // Fall through all the following color codes.
      // Set the color code now so it can be 
      // translated into an ansi color later.
      case 'r':
      case 'g':
      case 'b':
      case 'c':
      case 'm':
      case 'y':
      case 'k':
      case 'w':
        cmd_options->color = c;
        break;

      default:
        DEBUG_LOG("%d('%c') unused!\n", c, c);
        abort();
    }
  }
}

int main(int argc, const char *argv[])
{
  // The default line options.
  line_options_t line_options = { 
    .columns=DEFAULT_COLUMN_LEN,
    .dash=(char *)DEFAULT_DASH,
    .dash_len=sizeof(DEFAULT_DASH) - 1, // Ignore NULL Char
    .color=NULL
  };

  cmd_options_t cmd_options;
  memset(&cmd_options, 0, sizeof(cmd_options));
  check_options(argc, argv, &cmd_options);

  // should get from cmd_options instead
  switch (cmd_options.color) {
    case 0:   line_options.color = NULL; break;
    case 'r': line_options.color = ANSI_COLOR_RED; break;
    case 'g': line_options.color = ANSI_COLOR_GREEN; break;
    case 'b': line_options.color = ANSI_COLOR_BLUE; break;
    case 'y': line_options.color = ANSI_COLOR_YELLOW; break;
    case 'm': line_options.color = ANSI_COLOR_MAGENTA; break;
    case 'c': line_options.color = ANSI_COLOR_CYAN; break;
    case 'k': line_options.color = ANSI_COLOR_BLACK; break;
    case 'w': line_options.color = ANSI_COLOR_WHITE; break;
  }

  // Sets the text if the user set any.
  if (cmd_options.text) line_options.text = cmd_options.text;

  // Sets the program's locale so functions like mblen function properly.
  setlocale(LC_CTYPE, "");

  if (cmd_options.force_columns) {
    line_options.columns = cmd_options.force_columns;
  }
  else {
    line_options.columns = get_column_width_from_term();
  }

  if (cmd_options.force_dash) {
    line_options.dash = cmd_options.force_dash;
  }
  else {
    // Switches default to unicode default.
    bool unicode_supported = is_unicode_supported();
    if (unicode_supported) {
      line_options.dash = (char *) DEFAULT_DASH_UNICODE;
      line_options.dash_len = strlen(DEFAULT_DASH_UNICODE);
    }
  }

  drawLine(&line_options);
  return 0;
}

