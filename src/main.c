#include "corganizer.h"

#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "error argument!\n"); return 1;
  }

  return corganizer(argv[1]) ? 0 : 1;
}
