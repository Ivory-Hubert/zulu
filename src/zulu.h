#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>


#define SHOW_VER 0
#define SHOW_HELP 1

#define KIB (1ULL << 10)
#define MIB (1ULL << 20)
#define GIB (1ULL << 30)

#define HALF_KIB (KIB >> 1)
#define HALF_MIB (MIB >> 1)
#define HALF_GIB (GIB >> 1)

typedef uint64_t u64;


static const char yellow[] = "\x1b[38;5;222m";
static const char green[] = "\x1b[38;5;78m";
static const char grey[] = "\x1b[38;5;8m";
static const char red[] = "\x1b[38;5;160m";
static const char cyan[] = "\x1b[38;5;14m";
static const char reset[] = "\x1b[m";

extern volatile int lite_mode;
extern volatile int list_files;
extern volatile int byte_list;

extern clock_t beginning;

extern char biggest[1024];
extern char smallest[1024];


struct fileParam {
    u64 file_count;
    u64 folder_count;
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
    u64 folder_count;
};

// zulu.c
void listBytes(int argc, char **argv, const char *cwd);
void listAll(int argc, char **argv, const char *cwd);
void listPath(int argc, char **argv);
void convert(int argc, char **argv);
void auditFile(int argc, char **argv, const char *cwd);

// core.c
int searchFolder(const char *path);
void sizeMath(struct fileParam *fpp);
void byteMath(const char *raw);
void fileData(const char *path);

// ui.c
void stdout_ui(int mode);
void display(struct displayParam *dpp, int timer_ms);
void liteDisplay(struct displayParam *dpp, int timer_ms);
