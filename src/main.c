#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

#include "version.h"

static struct option getopt_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {"dot", no_argument, NULL, 'd'},
    {"line", optional_argument, NULL, 'l'},
    {"separator", required_argument, NULL, 's'},
    {NULL, 0, NULL, 0}
};

static char *app_name = "ext";

static bool opt_has_dot = false;
static char opt_print_separator = '\n';
static char opt_separator = '\0';

static int element_index = -1;

void print_help() {
  fprintf(stderr, "Print file (s) extension (s).\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "\text [options] [--] [filenames...]  read filenames from arguments\n");
  fprintf(stderr, "\text [options] -                    read filenames from stdin\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "\t-h       --help                    display this and exit\n");
  fprintf(stderr, "\t-V       --version                 display version info\n");
  fprintf(stderr, "\t-d       --dot                     print dot before every extension\n");
  fprintf(stderr, "\t-l [SEP] --line=[SEP]              print output in single line with specified separator [default: ' ']\n");
  fprintf(stderr, "\t-s [SEP] --separator=[SEP]         input separator (only if reading from stdin) [default: ' ']\n");
}

void print_version() {
  fprintf(stderr, "ext version %s\n", VERSION);
  fprintf(stderr, "support https://github.com/Tarik02/ext\n");

  if (GIT_COMMIT_SHA1 == NULL) {
    fprintf(stderr, "no commit info found\n");
  } else {
    fprintf(stderr, "built from commit %s (at %s):\n%s\n",
      GIT_COMMIT_SHA1,
      GIT_COMMIT_DATE,
      GIT_COMMIT_MESSAGE
    );
  }
}

void print_element(const char *elem) {
  if (element_index != 0) {
    fputc(opt_print_separator, stdout);
  }

  if (opt_has_dot) {
    fputc('.', stdout);
  }

  fprintf(stdout, "%s", elem);
}

void error_element(const char *elem, const char *error) {
  fprintf(stdout, "\n");
  fprintf(stderr, "Failed to process element '%s': %s\n", elem, error);
}

void process_element(const char *e) {
  ++element_index;

  size_t len = strlen(e);

  for (size_t i = len; i != 0; ) {
    --i;

    if (e[i] == '.') {
      print_element(e + i + 1);
      return;
    }
  }

  error_element(e, "there is no '.' in filename");
}

int main(int argc, char *argv[]) {
  app_name = argv[0];

  int ch;
  while ((ch = getopt_long(argc, argv, "hVdls:", getopt_options, NULL)) != -1) {
    switch (ch) {
    case 'h':
      print_help();
      return EXIT_FAILURE;
    case 'V':
      print_version();
      return EXIT_FAILURE;
    case 'd':
      opt_has_dot = true;
      break;
    case 'l':
      if (optarg == NULL) {
        opt_print_separator = ' ';
      } else {
        if (strlen(optarg) != 1) {
          print_help();
          fprintf(stderr, "%s: argument -l or --line value should be 1 character wide\n", app_name);
          return EXIT_FAILURE;
        }

        opt_print_separator = optarg[0];
      }
      break;
    case 's':
      if (strlen(optarg) != 1) {
        fprintf(stderr, "%s: separator should be 1 character wide\n", app_name);
        print_help();
        return EXIT_FAILURE;
      }

      opt_separator = optarg[0];
      break;
    default:
      print_help();
      return EXIT_FAILURE;
    }
  }

  if (optind + 1 == argc && strcmp(argv[optind], "-") == 0) {
    if (opt_separator == '\0') {
      opt_separator = '\n';
    }

    char *element = NULL;
    size_t element_size = 0;
    ssize_t len;

    while (-1 != (len = getdelim(&element, &element_size, opt_separator, stdin))) {
      // remove separator
      element[len - 1] = '\0';

      process_element(element);

      free(element);
    }
  } else {
    if (opt_separator != '\0') {
      fprintf(stderr, "%s: separator can only be specified if reading from stdin\n", app_name);
      print_help();
      return EXIT_FAILURE;
    }

    while (optind < argc) {
      process_element(argv[optind]);
      ++optind;
    }
  }

  if (opt_print_separator == '\n' && element_index != -1) {
    fputc(opt_print_separator, stdout);
  }

  return EXIT_SUCCESS;
}
