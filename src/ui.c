/* Zulu - fast & lightweight CLI for directory stats
 * Copyright (C) 2026 Ivori Huobolainen
 * Licensed under the GNU GPL v3 or later
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zulu.h"

static const char usage[] = 
    "Usage: zulu [-flag] [optional arg] <required arg>\n"
    "\n"
    "    -a                Minimal display in allocated sizes\n"            
    "    -s  [--ls] [PATH] Summary of the current or provided directory\n"
    "    -sa [--lss]       '-sa' shows allocated sizes on disk, otherwise behaves the same\n"
    "                      '--ls' also lists the files/directories counted\n"
    "                      '--lss' adds converted file sizes to the list\n"
    "\n"
    "    -l  [PATH]        List the current or chosen directory in a table format\n"
    "    -lh               'human' output flag, sizes shown in kib/mib/gib as needed\n"
    "                      When piped outputs with no conversion, formatting or colors\n"
    "\n"
    "    -c  <bytes>       Convert provided bytes and display results\n"
    "    -f  <PATH>        Detailed file stats, for a CWD file or provided file path\n"
    "    -h                Display this help message and exit\n"
    "    --help\n"
    "    -v                Display version number and exit\n"
    "\n"
    "If no flags are provided Zulu will display the bare minimum, in apparent sizes.\n"
    "Use the 'NO_COLOR' or 'ZULU_NO_COLOR' env. variables to toggle colors.\n"
    "\n";

static const char version[] = 
    "zulu - 0.1.4\n\n"
    "(C) 2026 Ivori Huobolainen\n"
    "Licensed under the GNU GPL v3 or later\n"
    "<https://www.gnu.org/licenses/gpl-3.0.html>\n";


void stdout_ui(int mode) {
    if (mode) { printf("%s", usage); }
    else { printf("%s", version); }
}

void display(struct displayParam *dpp, int timer_ms) {
    if (list_files) {
        printf("\n======= %sData%s =======",
            CYAN, RESET);
    }

    printf("\n[%sTotal%s]\n", CYAN, RESET);
    if (dpp->total_gb > 0) {
        printf(" * %s%lu%s GiB\n * %lu MiB\n",
            CYAN, dpp->total_gb, RESET,
            dpp->total_mb);
    }
    else if (dpp->total_mb > 0) {
        printf(" * %s%lu%s MiB\n * %lu KiB\n",
            CYAN, dpp->total_mb, RESET,
            dpp->total_kb);
    }
    else if (dpp->total_kb > 0) {
        printf(" * %s%lu%s KiB\n * %lu bytes\n",
            CYAN, dpp->total_kb, RESET,
            dpp->total_bytes);
    }
    else if (dpp->total_bytes > 0) {
        printf(" * %s%lu%s bytes\n",
            CYAN, dpp->total_bytes, RESET);
            
    } else {
        // early exit to not clutter UI
        printf("No files or empty file(s)\n");
        printf("\nFolders: %s%lu%s\n\n",
            CYAN, dpp->folder_count, RESET);

        exit(0);
    }


    printf("\n[%sLargest%s]\n", YELLOW, RESET);
    if (biggest[0]) printf(" - %s\n", biggest);

    if (dpp->biggest_gb > 0) {
        printf(" * %s%lu%s GiB\n * %lu MiB\n",
            YELLOW, dpp->biggest_gb, RESET,
            dpp->biggest_mb);
    }
    else if (dpp->biggest_mb > 0) {
        printf(" * %s%lu%s MiB\n * %lu KiB\n",
            YELLOW, dpp->biggest_mb, RESET,
            dpp->biggest_kb);
    }
    else if (dpp->biggest_kb > 0) {
        printf(" * %s%lu%s KiB\n * %lu bytes\n",
            YELLOW, dpp->biggest_kb, RESET,
            dpp->biggest_bytes);
            
    } else {
        printf(" * %s%lu%s bytes\n",
            YELLOW, dpp->biggest_bytes, RESET);
    }


    printf("\n[%sTiniest%s]\n", GREEN, RESET);
    if (smallest[0]) printf(" - %s\n", smallest);

    if (dpp->smallest_gb > 0) {
        printf(" * %s%lu%s GiB\n * %lu MiB\n",
            GREEN, dpp->smallest_gb, RESET,
            dpp->smallest_mb);
    }
    else if (dpp->smallest_mb > 0) {
        printf(" * %s%lu%s MiB\n * %lu KiB\n",
            GREEN, dpp->smallest_mb, RESET,
            dpp->smallest_kb);
    }
    else if (dpp->smallest_kb > 0) {
        printf(" * %s%lu%s KiB\n * %lu bytes\n",
            GREEN, dpp->smallest_kb, RESET,
            dpp->smallest_bytes);
            
    } else {
        printf(" * %s%lu%s bytes\n",
            GREEN, dpp->smallest_bytes, RESET);
    }
    

    printf("\nFiles:   %s%lu%s",
        CYAN, dpp->file_count, RESET);
    printf("\nFolders: %s%lu%s\n",
        CYAN, dpp->folder_count, RESET);


    static char mode[12];
    snprintf(mode, sizeof(mode), "%s",
        show_blocks ? "*Allocated" : "*Apparent");
        
    if (timer_ms > 0) {
        printf("\n%s (%s%d%s ms)\n",
            mode, CYAN, timer_ms, RESET);
    } else {
        printf("\n%s (< %s%d%s ms)\n",
            mode, CYAN, timer_ms, RESET);
    }

    
    if (list_files) {
        printf("====================\n");
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


    static char mode[12];
    snprintf(mode, sizeof(mode), "%s",
        show_blocks ? "*Allocated" : "*Apparent");
    
    if (timer_ms > 0) {
        printf("\n%s (%d ms)\n", mode, timer_ms);
    } else {
        printf("\n%s (< %d ms)\n", mode, timer_ms);
    }
    
    exit(0);
}

