#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>

#define KIB (1ULL << 10)
#define MIB (1ULL << 20)
#define GIB (1ULL << 30)

typedef uint64_t u64;

static const char red[] = "\x1b[38;5;196m";
static const char cyan[] = "\x1b[38;5;14m";
static const char reset[] = "\x1b[m";

extern volatile int lite_mode;
extern volatile int list_files;
extern volatile int byte_list;

extern char biggest[1024];
extern char smallest[1024];

struct fileParam {
    u64 file_count;
    u64 total_size;
    u64 min_size;
    u64 max_size;
};

struct displayParam {
    u64 biggest_bytes;
    u64 biggest_kb;
    u64 biggest_mb;
    u64 biggest_gb;
    u64 smallest_bytes;
    u64 smallest_kb;
    u64 smallest_mb;
    u64 smallest_gb;
    u64 total_bytes;
    u64 total_kb;
    u64 total_mb;
    u64 total_gb;
    u64 file_count;
};

int listBytes(int argc, char **argv, const char *cwd);
int listAll(int argc, char **argv, const char *cwd);
int listPath(int argc, char **argv);

int searchFolder(const char *path);
void sizeMath(struct fileParam *fpp);

// in ui.c
void showHelp();
void display(struct displayParam *dpp, int timer_ms);
void liteDisplay(struct displayParam *dpp, int timer_ms);
