#ifndef CORGANIZER_H
#define CORGANIZER_H

#include <stdbool.h>

typedef struct {
  const char *extension;
  const char *category;
} TypeFile;

const char *get_category(const char file_name[]);
bool corganizer(const char path[]);

#endif // !CORGANIZER_H
