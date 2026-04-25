/* Zulu - fast & lightweight CLI for directory stats
 * Copyright (C) 2026 Ivori Huobolainen
 * Licensed under the GNU GPL v3 or later
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zulu.h"


int searchFolder(const char *path) {
    struct dirent *de;
    DIR *dr = opendir(path);

    if (dr == NULL) {
        perror("zulu> error");
        return 0;
    }

    struct fileParam fpp = { 0 };

    fpp.min_size = (u64)-1;
    // to have a base line higher than any normal file size
    // the moment a real file gets counted its gone, replaced
    
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

        if ((st.st_mode & S_IFMT) == S_IFDIR) {
            fpp.folder_count++;

            if (list_files) {
                printf(" %s%s/%s\n", grey, de->d_name, reset);
            }
        }
        
        if ((st.st_mode & S_IFMT) != S_IFREG) {
            continue;
        }

        if ((u64)st.st_size > (1ULL << 40)) {
            printf("%szulu%s> '%s' skipped\n", red, reset, de->d_name);
            continue;
        }

        size_t size = show_blocks ? st.st_blocks : st.st_size;
        
        if(!lite_mode && !byte_list) {
            if (fpp.file_count == 0) {
                fpp.min_size = size;
                fpp.max_size = size;

                snprintf(biggest, sizeof(biggest), "%s", de->d_name);
                snprintf(smallest, sizeof(smallest), "%s", de->d_name);
            }

            if (size > fpp.max_size) {
                fpp.max_size = size;

                snprintf(biggest, sizeof(biggest), "%s", de->d_name);
            }
            if (size < fpp.min_size) {
                fpp.min_size = size;
                
                snprintf(smallest, sizeof(smallest), "%s", de->d_name);
            }
            
            if (list_files) printf(" - %s\n", de->d_name);
        }

        if (byte_list) {
            if (isatty(STDOUT_FILENO)) {
                printf(" - %-32.32s | %s%lu%s\n",
                    de->d_name,
                    cyan, st.st_size, reset);
            } else {
                printf("%s|%lu\n",
                    de->d_name,
                    st.st_size);
            }
        }
        
        fpp.file_count++;
        fpp.total_size += size;
    }

    closedir(dr);
    if (!byte_list) sizeMath(&fpp);
    return fpp.file_count;
}

void sizeMath(struct fileParam *fpp) {
    struct displayParam dpp = { 0 };

    //if (fpp->file_count == 0) fpp->min_size = 0;
    
    dpp.file_count = fpp->file_count;
    dpp.folder_count = fpp->folder_count;

    if (show_blocks) {
        fpp->total_size = fpp->total_size * 512;

        if (!lite_mode) {
            fpp->max_size = fpp->max_size * 512;
            fpp->min_size = fpp->min_size * 512;
        }
    }

    
    if (fpp->total_size >= GIB) {
        dpp.total_gb = (fpp->total_size + HALF_GIB) / GIB;
        dpp.total_mb = (fpp->total_size + HALF_MIB) / MIB;
    }
    else if (fpp->total_size >= MIB) {
        dpp.total_mb = (fpp->total_size + HALF_MIB) / MIB;
        dpp.total_kb = (fpp->total_size + HALF_KIB) / KIB;
    }
    else if (fpp->total_size >= KIB) {
        dpp.total_kb = (fpp->total_size + HALF_KIB) / KIB;
        dpp.total_bytes = fpp->total_size;
        
    } else {
        dpp.total_bytes = fpp->total_size;
    }

    
    if (!lite_mode) {
        if (fpp->max_size >= GIB) {
            dpp.biggest_gb = (fpp->max_size + HALF_GIB) / GIB;
            dpp.biggest_mb = (fpp->max_size + HALF_MIB) / MIB;
        }
        else if (fpp->max_size >= MIB) {
            dpp.biggest_mb = (fpp->max_size + HALF_MIB) / MIB;
            dpp.biggest_kb = (fpp->max_size + HALF_KIB) / KIB;
        }
        else if (fpp->max_size >= KIB) {
            dpp.biggest_kb = (fpp->max_size + HALF_KIB) / KIB;
            dpp.biggest_bytes = fpp->max_size;
            
        } else {
            dpp.biggest_bytes = fpp->max_size;
        }


        if (fpp->min_size >= GIB) {
            dpp.smallest_gb = (fpp->min_size + HALF_GIB) / GIB;
            dpp.smallest_mb = (fpp->min_size + HALF_MIB) / MIB;
        }
        else if (fpp->min_size >= MIB) {
            dpp.smallest_mb = (fpp->min_size + HALF_MIB) / MIB;
            dpp.smallest_kb = (fpp->min_size + HALF_KIB) / KIB;
        }
        else if (fpp->min_size >= KIB) {
            dpp.smallest_kb = (fpp->min_size + HALF_KIB) / KIB;
            dpp.smallest_bytes = fpp->min_size;
            
        } else {
            dpp.smallest_bytes = fpp->min_size;
        }
    }

    clock_t elapsed = clock() - beginning;
    int timer_ms  = (elapsed * 1000) / CLOCKS_PER_SEC;
    
    if (lite_mode) liteDisplay(&dpp, timer_ms);
    display(&dpp, timer_ms);
}

void byteMath(const char *raw) {
    errno = 0;
    char *endptr;

    unsigned long long tmp = strtoull(raw, &endptr, 10);
    
    if (errno == ERANGE) {
        perror("zulu> error");
        return;
    }

    if (endptr == raw) {
        fprintf(stderr, "zulu> No digits found\n");
        return;
    }

    u64 val = (u64)tmp;
    
    u64 gib = (val + HALF_GIB) / GIB;
    u64 mib = (val + HALF_MIB) / MIB;
    u64 kib = (val + HALF_KIB) / KIB;

    printf("zulu> Results:\n");
    printf(" * %s%lu%s GiB\n * %s%lu%s MiB\n * %s%lu%s KiB\n\n",
        cyan, gib, reset,
        cyan, mib, reset,
        cyan, kib, reset);
        
}

void fileData(const char *path) {
    struct stat st;

    if (stat(path, &st) == -1) {
        perror("zulu> error");
        return;
    }

    printf("File type: ");
    
    switch (st.st_mode & S_IFMT) {
        case S_IFBLK:
            printf("block device\n");
            break;
            
        case S_IFCHR:
            printf("character device\n");
            break;
            
        case S_IFDIR:
            printf("directory\n");
            break;
            
        case S_IFIFO:
            printf("FIFO/pipe\n");
            break;
            
        case S_IFLNK:
            printf("symlink\n");
            break;
            
        case S_IFREG:
            printf("regular file\n");
            break;
            
        case S_IFSOCK:
            printf("socket\n");
            break;
            
        default:
            printf("unknown?\n");
            break;
    }

    printf("\nID of containing device: [%x,%x]\n",
        major(st.st_dev), minor(st.st_dev));
    
    printf("\nLast access:        %s\n", ctime(&st.st_atime));
    printf("Last modification:  %s\n", ctime(&st.st_mtime));
    printf("Last status change: %s\n", ctime(&st.st_ctime));

    printf("Ownership:   UID=%ju   GID=%ju\n",
        (uintmax_t)st.st_uid, (uintmax_t)st.st_gid);

    printf("\nI-node number: %ju\n", (uintmax_t)st.st_ino);
    printf("Mode:          %jo (octal)\n", (uintmax_t)st.st_mode);
    printf("Link count:    %ju\n", (uintmax_t)st.st_nlink);

    printf("\nPreferred I/O block size: %jd bytes\n",
        (intmax_t)st.st_blksize);
    printf("File size:                %jd bytes\n",
        (intmax_t)st.st_size);
    printf("Blocks allocated:         %jd\n",
        (intmax_t)st.st_blocks);
    
}

