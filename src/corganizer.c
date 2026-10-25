#include "corganizer.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <limits.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

static const TypeFile file_type[] = {
  // for image
  {".jpg", "Image"}, {".png", "Image"}, {".jpeg", "Image"}, {".heic", "Image"},

  // for source code
  {".c", "Code"}, {".cc", "Code"}, {".py", "Code"}, {".rs", "Code"},
  {".java", "Code"}, {".js", "Code"},

  // video and music
  {".mp4", "Video"}, {".mp3", "Music"},

  // documents
  {".docx", "Documents"}, {".pdf", "Documents"}, {".xlsx", "Documents"}, {".pptx", "Documents"}, 
  {".txt", "Documents"},

  // data files
  {".json", "Data_files"}, {".xml", "Data_files"}, {".csv", "Data_files"},
  // archives
  {".zip", "Archives"}, {".tar", "Archives"}, {".gz", "Archives"},
  // iso file
  {".iso", "Iso_files"}
};

const char *get_category(const char file_name[]) {

  const char *ext = strrchr(file_name, '.');
  if (!ext) return "Other_files";

  size_t n = sizeof(file_type) / sizeof(file_type[0]);

  for (size_t i = 0; i < n; i++) {
    if (strcasecmp(ext, file_type[i].extension) == 0) {
      return file_type[i].category;
    }
  }

  return "Other_files";
}

bool corganizer(const char path[]) {
  // open dir path
  DIR *dir = opendir(path);

  if (!dir) {
    perror("opendir has failed!"); return false;
  }
  bool success = true;

  struct dirent *entry;
  // skip file './' and '../' in a dir path
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char source[PATH_MAX];

    int buff_size = snprintf(source, sizeof(source), "%s/%s", path, entry->d_name);
    if (buff_size < 0 || (size_t)buff_size >= sizeof(source)) {
      fprintf(stderr, "path too long or formating error!\n");
      success = false;
      continue;
    }

    struct stat source_info;
    // inspect source entry
    if (lstat(source, &source_info) != 0) {
      perror("get a status file failed!"); 
      success = false; continue;
    }

    if (!S_ISREG(source_info.st_mode)) continue;

    const char *category = get_category(entry->d_name);

    char target_dir[PATH_MAX];
    buff_size = snprintf(target_dir, sizeof(target_dir), "%s/%s", path, category);

    if (buff_size < 0 || (size_t)buff_size >= sizeof(target_dir)) {
      fprintf(stderr, "path too long or formating error!\n");
      success = false;
      continue;
    }

    // make a directory
    if (mkdir(target_dir, 0755) != 0 && errno != EEXIST) {
      perror("make directory has failed");
      success = false; continue;
    }

    // verify category path
    struct stat target_dir_info;
    if (stat(target_dir, &target_dir_info) != 0) {
      perror("target directory is not a directory"); 
      success = false; continue;
    }

    if (!S_ISDIR(target_dir_info.st_mode)) {
      fprintf(stderr, "path is not directory!\n");
      success = false; continue;
    }

    // build a destination path
    char target[PATH_MAX];
    buff_size = snprintf(target, sizeof(target), "%s/%s", target_dir, entry->d_name);

    if (buff_size < 0 || (size_t)buff_size >= sizeof(target)) {
      fprintf(stderr, "path too long or formating error!\n"); 
      success = false; continue;
    }

    // check wheather the destination path already exists
    struct stat target_info;
    if (lstat(target, &target_info) == 0) {
      printf("skip tergets %s\n", entry->d_name); continue;
    }

    if (errno != ENOENT) {
      perror("lstat target"); 
      success = false; continue;
    }

    // move the file
    if (rename(source, target) != 0) {
      perror("rename file has failed!");
      success = false; continue;
    }

    printf("MOVE: [%s] to [%s]\n", entry->d_name, category);
  }

  if (closedir(dir) != 0) {
    perror("closedir has failed!"); success = false;
  }
  return success;
}
