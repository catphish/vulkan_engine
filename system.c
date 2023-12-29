#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "system.h"

FileData file_read(char* path) {
  FileData fileData;
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    ERROR("Failed to open file: %s\n", path);
  }
  fileData.size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  fileData.data = malloc(fileData.size);
  int n = read(fd, fileData.data, fileData.size);
  if (n < fileData.size) {
    printf("Failed to read file %s!\n", path);
  }
  close(fd);
  return fileData;
}
