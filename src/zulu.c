/* Zulu - fast & lightweight CLI for directory stats
 * Copyright (C) 2026 Ivori Huobolainen
 * Licensed under the GNU GPL v3 or later
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zulu.h"

volatile int lite_mode = 0;
volatile int list_files = 0;
volatile int byte_list = 0;

static clock_t beginning;

char biggest[1024] = { 0 };
char smallest[1024] = { 0 };

int main(int argc, char **argv) {
    beginning = clock();
    char *cwd = getcwd(NULL, 0);
    
    if (argc < 2) {
        printf("zulu> Type 'zulu -h' for help\n");

        lite_mode = 1;
        searchFolder(cwd);
    }
    else if (strcmp(argv[1], "-h") == 0) {
        showHelp();
    }
    else if (strcmp(argv[1], "-v") == 0) {
        showVer();
    }
    else if (strcmp(argv[1], "-b") == 0) {
        listBytes(argc, argv, cwd);
    }
    else if (strcmp(argv[1], "-s") == 0) {
        listAll(argc, argv, cwd);
    }
    else if (strcmp(argv[1], "-p") == 0) {
        listPath(argc, argv);
    } else {
        printf("%szulu%s> Unknown parameter '%s'\n", red, reset, argv[1]);
    }
    
    return 0;
}

int listBytes(int argc, char **argv, const char *cwd) {
    // zulu -b (PATH)
    byte_list = 1;

    if (argc > 2) {
        printf("Listing: %s\n\n", argv[2]);
        int count = searchFolder(argv[2]);

        printf("\nCount: %d\n", count);
        return 0;
    };

    printf("Listing: %s\n\n", cwd);
    int count = searchFolder(cwd);
        
    printf("\nCount: %d\n", count);
    return 0;
}

int listAll(int argc, char **argv, const char *cwd) {
    // zulu -s (--ls)
    printf("%szulu%s> Showing '%s'\n", cyan, reset, cwd);

    if (argc > 2 && strcmp(argv[2], "--ls") == 0) {
        list_files = 1;
        printf("\n==== %sFolder list%s ====\n", cyan, reset);
    }
        
    searchFolder(cwd);
    return 0;
}

int listPath(int argc, char **argv) {
    // zulu -p (--ls) PATH
    if (argc < 3) {
        printf("%szulu%s> No directory provided!\n", red, reset);
        return 0;
    }

    volatile int arg_type = 0;
        
    if (argc > 3 && strcmp(argv[2], "--ls") == 0) {
        list_files = 1;
        arg_type = 1;
    }

    if (arg_type) {
        printf("%szulu%s> Showing '%s'\n", cyan, reset, argv[3]);
        printf("\n==== %sFolder list%s ====\n", cyan, reset);
        searchFolder(argv[3]);
    } else {
        printf("%szulu%s> Showing '%s'\n", cyan, reset, argv[2]);
    }

    searchFolder(argv[2]);
    return 0;
}


int searchFolder(const char *path) {
    struct dirent *de;
    DIR *dr = opendir(path);

    if (dr == NULL) {
        perror("zulu> error");
        return 0;
    }

    struct fileParam fpp;

    fpp.file_count = 0;
    fpp.total_size = 0;
    fpp.min_size = (u64)-1;
    // to have a base line higher than any normal file size
    // the moment a real file gets counted its gone, replaced
    fpp.max_size = 0;
    
    char full_path[4096];
    
    while ((de = readdir(dr)) != NULL) {
        if (!strcmp(de->d_name, ".") ||
            !strcmp(de->d_name, ".."))
            continue;

        if (snprintf(full_path, sizeof(full_path), "%s/%s", path, de->d_name)
                >= (int)sizeof(full_path)) {                
            fprintf(stderr, "zulu> File path too long, terminated.\n");
            exit(1);
        }
        
        struct stat st;
        if (stat(full_path, &st) == -1) {
            continue;
        }
        
        if ((st.st_mode & S_IFMT) != S_IFREG) {
            continue;
        }

        if ((u64)st.st_size > (1ULL << 40)) {
            printf("\n%szulu%s> absurd file size... '%s' skipped\n", red, reset, de->d_name);
            continue;
        }
        
        if(!lite_mode) {
            if (fpp.file_count == 0) {
                fpp.min_size = st.st_size;
                fpp.max_size = st.st_size;

                snprintf(biggest, sizeof(biggest), "%s", de->d_name);
                snprintf(smallest, sizeof(smallest), "%s", de->d_name);
            }

            if ((u64)st.st_size > fpp.max_size) {
                fpp.max_size = st.st_size;
                snprintf(biggest, sizeof(biggest), "%s", de->d_name);
            }
            if ((u64)st.st_size < fpp.min_size) {
                fpp.min_size = st.st_size;
                snprintf(smallest, sizeof(smallest), "%s", de->d_name);
            }
            
            if (list_files) printf(" - %s\n", de->d_name);
        }

        if (byte_list) {
            if (isatty(STDOUT_FILENO)) {
                printf(" - %-32.32s | %s%ld%s bytes\n"
                , de->d_name, cyan, st.st_size, reset);
            } else {
                printf("%s|%ld\n"
                , de->d_name, st.st_size);
            }
        }
        
        fpp.file_count++;
        fpp.total_size += st.st_size;
    }

    closedir(dr);
    if (!byte_list) sizeMath(&fpp);
    return fpp.file_count;
}

void sizeMath(struct fileParam *fpp) {
    struct displayParam dpp;

    dpp.biggest_bytes = 0;
    dpp.biggest_kb = 0;
    dpp.biggest_mb = 0;
    dpp.biggest_gb = 0;
    
    dpp.smallest_bytes = 0;
    dpp.smallest_kb = 0;
    dpp.smallest_mb = 0;
    dpp.smallest_gb = 0;
    
    dpp.total_bytes = 0;
    dpp.total_kb = 0;
    dpp.total_mb = 0;
    dpp.total_gb = 0;
    
    dpp.file_count = 0;

    // crucial, since min_size is massive when a dir is empty
    if (fpp->file_count == 0) {
        fpp->total_size = 0;
        fpp->max_size = 0;
        fpp->min_size = 0;
    }

    dpp.file_count = fpp->file_count;

    if (fpp->total_size >= 1073741824) {
        dpp.total_gb = (fpp->total_size + 512) / GIB;
        dpp.total_mb = (fpp->total_size + 512) / MIB;
    }
    else if (fpp->total_size >= 1024) {
        dpp.total_mb = (fpp->total_size + 512) / MIB;
        dpp.total_kb = (fpp->total_size + 512) / KIB;
    }
    else {
        dpp.total_bytes = fpp->total_size;
    }

    if (!lite_mode) {
        if (fpp->max_size < 1024) {
            dpp.biggest_bytes = fpp->max_size;
        }
        else if (fpp->max_size >= 1073741824) {
            dpp.biggest_gb = fpp->max_size / GIB;
        }

        dpp.biggest_mb = fpp->max_size / MIB;
        dpp.biggest_kb = fpp->max_size / KIB;


        if (fpp->min_size < 1024) {
            dpp.smallest_bytes = fpp->min_size;
        }    
        else if (fpp->min_size >= 1073741824) {
            dpp.smallest_gb = fpp->min_size / GIB;
        }

        dpp.smallest_mb = fpp->min_size / MIB;
        dpp.smallest_kb = fpp->min_size / KIB;

    }

    clock_t elapsed = clock() - beginning;
    int timer_ms  = (elapsed * 1000) / CLOCKS_PER_SEC;
    
    if (lite_mode) liteDisplay(&dpp, timer_ms);
    display(&dpp, timer_ms);
}

