/* Zulu - fast & lightweight CLI for directory stats
 * Copyright (C) 2026 Ivori Huobolainen
 * Licensed under the GNU GPL v3 or later
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zulu.h"


static const char usage[] = 
    "Usage: zulu [-flag] [optional arg] <required arg>\n\n"
    "    -s [--ls]         Summary of current directory\n"
    "    -p [--ls] <PATH>  Summary of provided directory\n"
    "    -b [PATH]         List the current or chosen directory with byte sizes\n"
    "    -c <bytes>        Convert provided bytes and display results\n"
    "    -f <PATH>         Detailed file data, for a CWD file or provided file path\n"
    "    -h                Display this help message and exit\n"
    "    -v                Display version number and exit\n"
    "\nOptional '--ls' flag will also list the files/folders that Zulu counts.\n"
    "\nIf no flags are provided Zulu will display the bare minimum, without colors.\n"
    "\n";

static const char version[] = 
    "zulu - 0.1.2\n\n"
    "(C) 2026 Ivori Huobolainen\n"
    "Licensed under the GNU GPL v3 or later\n"
    "<https://www.gnu.org/licenses/gpl-3.0.html>\n";


void stdout_ui(int mode) {
    if (mode) {
        printf("%s", usage);
    }
    else {
        printf("%s", version);
    }
}

void display(struct displayParam *dpp, int timer_ms) {
    if (list_files) {
        printf("\n==== %sFolder Data%s ====",
            cyan, reset);
    }

    printf("\n[%sTotal%s]\n", cyan, reset);
    if (dpp->total_gb > 0) {
        printf(" * %s%lu%s GiB\n * %lu MiB\n",
            cyan, dpp->total_gb, reset,
            dpp->total_mb);
    }
    else if (dpp->total_mb > 0) {
        printf(" * %s%lu%s MiB\n * %lu KiB\n",
            cyan, dpp->total_mb, reset,
            dpp->total_kb);
    }
    else if (dpp->total_kb > 0) {
        printf(" * %s%lu%s KiB\n * %lu bytes\n",
            cyan, dpp->total_kb, reset,
            dpp->total_bytes);
    }
    else if (dpp->total_bytes > 0) {
        printf(" * %s%lu%s bytes\n",
            cyan, dpp->total_bytes, reset);
            
    } else {
        // early exit to not clutter UI
        printf("No files or empty file(s)\n");
        printf("\nFolders: %s%lu%s\n\n",
            cyan, dpp->folder_count, reset);

        exit(0);
    }


    printf("\n[%sLargest%s]\n", yellow, reset);
    if (biggest[0]) printf(" - %s\n", biggest);

    if (dpp->biggest_gb > 0) {
        printf(" * %s%lu%s GiB\n * %lu MiB\n",
            yellow, dpp->biggest_gb, reset,
            dpp->biggest_mb);
    }
    else if (dpp->biggest_mb > 0) {
        printf(" * %s%lu%s MiB\n * %lu KiB\n",
            yellow, dpp->biggest_mb, reset,
            dpp->biggest_kb);
    }
    else if (dpp->biggest_kb > 0) {
        printf(" * %s%lu%s KiB\n * %lu bytes\n",
            yellow, dpp->biggest_kb, reset,
            dpp->biggest_bytes);
            
    } else {
        printf(" * %s%lu%s bytes\n",
            yellow, dpp->biggest_bytes, reset);
    }


    printf("\n[%sTiniest%s]\n", green, reset);
    if (smallest[0]) printf(" - %s\n", smallest);

    if (dpp->smallest_gb > 0) {
        printf(" * %s%lu%s GiB\n * %lu MiB\n",
            green, dpp->smallest_gb, reset,
            dpp->smallest_mb);
    }
    else if (dpp->smallest_mb > 0) {
        printf(" * %s%lu%s MiB\n * %lu KiB\n",
            green, dpp->smallest_mb, reset,
            dpp->smallest_kb);
    }
    else if (dpp->smallest_kb > 0) {
        printf(" * %s%lu%s KiB\n * %lu bytes\n",
            green, dpp->smallest_kb, reset,
            dpp->smallest_bytes);
            
    } else {
        printf(" * %s%lu%s bytes\n",
            green, dpp->smallest_bytes, reset);
    }
    

    printf("\nFiles:   %s%lu%s",
        cyan, dpp->file_count, reset);
    printf("\nFolders: %s%lu%s\n",
        cyan, dpp->folder_count, reset);
    
    
    if (timer_ms > 0) {
        printf("\n(Took %s%d%s ms)\n",
            cyan, timer_ms, reset);
    } else {
        printf("\n(Took < %s%d%s ms)\n",
            cyan, timer_ms, reset);
    }

    
    if (list_files) {
        printf("=====================\n");
    }
    
    exit(0);
}

void liteDisplay(struct displayParam *dpp, int timer_ms) {
    printf("\nTotal size:");
    if (dpp->total_gb > 0) {
        printf("\n * %lu GiB\n * %lu MiB\n",
            dpp->total_gb, dpp->total_mb);
    }
    else if (dpp->total_mb > 0) {
        printf("\n * %lu MiB\n * %lu KiB\n",
            dpp->total_mb, dpp->total_kb);
    }
    else if (dpp->total_kb > 0) {
        printf("\n * %lu KiB\n * %lu bytes\n",
            dpp->total_kb, dpp->total_bytes);
    }
    else if (dpp->total_bytes > 0) {
        printf("\n * %lu bytes\n", dpp->total_bytes);
            
    } else {
        printf(" no files or empty file(s)\n");
    }

    
    printf("\nFiles:   %lu", dpp->file_count);
    printf("\nFolders: %lu\n", dpp->folder_count);

    
    if (timer_ms > 0) {
        printf("\n(Took %d ms)\n", timer_ms);
    } else {
        printf("\n(Took < %d ms)\n", timer_ms);
    }
    
    exit(0);
}

