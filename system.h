#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct fileData {
  uint32_t size;
  char* data;
} FileData;

#define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) printf(fmt, ##__VA_ARGS__); exit(EXIT_FAILURE)

FileData file_read(char* path);
