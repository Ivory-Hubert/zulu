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


#define SHOW_VER 1
#define SHOW_HELP 2
#define SHOW_SUM_UI 3
#define SHOW_LIST_UI 4

#define KIB (1ULL << 10)
#define MIB (1ULL << 20)
#define GIB (1ULL << 30)

#define HALF_KIB (KIB >> 1)
#define HALF_MIB (MIB >> 1)
#define HALF_GIB (GIB >> 1)

typedef uint64_t u64;

extern volatile int no_colors;
extern volatile int is_piped;
extern volatile int lite_mode;
extern volatile int list_files;
extern volatile int byte_list;
extern volatile int show_blocks;
extern volatile int human_sizes;

extern clock_t beginning;
extern char biggest[1024];
extern char smallest[1024];


#define ANSI_ITALIC "\x1b[3m"
#define ANSI_RESET  "\x1b[m"
#define ANSI_YELLOW "\x1b[38;5;222m"
#define ANSI_GREEN  "\x1b[38;5;78m"
#define ANSI_GREY   "\x1b[38;5;242m"
#define ANSI_RED    "\x1b[38;5;160m"
#define ANSI_CYAN   "\x1b[38;5;14m"
#define ANSI_BOLD   "\x1b[1m"

#define ITALIC (is_piped ? "" : ANSI_ITALIC)
#define RESET  (is_piped ? "" : ANSI_RESET)
#define YELLOW (is_piped ? "" : no_colors ? ANSI_BOLD : ANSI_YELLOW)
#define GREEN  (is_piped ? "" : no_colors ? ANSI_BOLD : ANSI_GREEN)
#define CYAN   (is_piped ? "" : no_colors ? ANSI_BOLD : ANSI_CYAN)
#define RED    (is_piped ? "" : no_colors ? ANSI_BOLD : ANSI_RED)
#define GREY   (no_colors ? "" : ANSI_GREY)


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

struct humanParam {
    char color[16];
    char unit[6];
    u64 convert;
};


// zulu.c
void listBytes(int argc, char **argv, const char *cwd);
void summary(int argc, char **argv, const char *cwd);
void convert(int argc, char **argv);
void auditFile(int argc, char **argv, const char *cwd);

// core.c
int searchFolder(int ui_mode, const char *path);
void sizeMath(struct fileParam *fpp);
void byteMath(const char *raw);
void humanOutput(struct humanParam *hpp, size_t size);
void fileData(const char *path);

// ui.c
void stdout_ui(int mode, const char *path);
void display(struct displayParam *dpp, int timer_ms);
void liteDisplay(struct displayParam *dpp, int timer_ms);

