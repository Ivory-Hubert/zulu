/* Zulu - fast & lightweight CLI for directory stats
 * Copyright (C) 2026 Ivori Huobolainen
 * Licensed under the GNU GPL v3 or later
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zulu.h"

static const char usage[] = 
    "Usage: zulu [MODE] [OPTIONS] [[PATH] <bytes>]\n"
    "\n"
    "Modes:\n"            
    "    -s  [PATH]     Summary of the current or provided directory\n"
    "        -a     Show allocated sizes on disk instead\n"
    "        --ls   Also list the files/directories counted\n"
    "        --lsu  Add converted size units to the list\n"
    "    -l  [PATH]     List the current or chosen directory in a table format,\n"
    "                   when piped outputs with no conversion and formatting\n"
    "        -h     Convert and print sizes in a more human readable way\n"
    "\n"
    "Standalone:\n"
    "    -a             Default minimal display, but in allocated sizes\n"
    "    -c  <bytes>    Convert provided bytes and display results\n"
    "    -f  <PATH>     Detailed file stats, for a CWD file or provided file path\n"
    "    -H, --help     Display this help message and exit\n"
    "    -V, --version  Display version number and exit\n"
    "\n"
    "Global options:\n"
    "        --color    Override the env. var. color state (see below)\n"
    "        --si       Show SI units instead (powers of 1000)\n"
    "\n"
    "If no flags are provided Zulu will display the bare minimum, in apparent sizes.\n"
    "The optional flags only work as explained above, beyond that flags don't mix,\n"
    "but for example '-lh' would work.\n"
    "\n"
    "Use the 'NO_COLOR' or 'ZULU_NO_COLOR' env. variables to toggle colors,\n"
    "'--color' will override the option when needed, but there's no need to manually\n"
    "manipulate color for piping/redirecting output.\n";
    
static const char version[] = 
    "zulu - 0.2.0\n\n"
    "(C) 2026 Ivori Huobolainen\n"
    "Licensed under the GNU GPL v3 or later\n"
    "<https://www.gnu.org/licenses/gpl-3.0.html>\n";


void stdout_ui(int mode, const char* path) {

    if (mode == SHOW_VER) {
        printf("%s", version);
    }
    else if (mode == SHOW_HELP) {
        printf("%s", usage);
    }
    else if (mode == SHOW_SUM_UI) {
        printf("zulu%s>%s '%s'\n", CYAN, RESET, path);

        if (list_files)
            printf("\n======= %sList%s =======\n", CYAN, RESET);
            
    }
    else if (mode == SHOW_LIST_UI) {
        printf("Listing: %s\n\n", path);

        if (!is_piped) {
            printf("  Sizes     | File\n");
            printf(" ------     |-----\n");
        }
        
    } else {
        return;
    }
}

void display(struct displayParam *dpp, int timer_ms) {
    static char mode[12];
    
    if (list_files) {
        printf("\n======= %sData%s =======",
            CYAN, RESET);
    }

    printf("\n[%sTotal%s]\n", CYAN, RESET);
    if (dpp->total_gb > 0) {
        printf(" * %s%lu%s %s\n * %lu %s\n",
            CYAN, dpp->total_gb, RESET,
            UNIT_GIB, dpp->total_mb, UNIT_MIB);
    }
    else if (dpp->total_mb > 0) {
        printf(" * %s%lu%s %s\n * %lu %s\n",
            CYAN, dpp->total_mb, RESET,
            UNIT_MIB, dpp->total_kb, UNIT_KIB);
    }
    else if (dpp->total_kb > 0) {
        printf(" * %s%lu%s %s\n * %lu bytes\n",
            CYAN, dpp->total_kb, RESET,
            UNIT_KIB, dpp->total_bytes);
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
        printf(" * %s%lu%s %s\n * %lu %s\n",
            YELLOW, dpp->biggest_gb, RESET,
            UNIT_GIB, dpp->biggest_mb, UNIT_MIB);
    }
    else if (dpp->biggest_mb > 0) {
        printf(" * %s%lu%s %s\n * %lu %s\n",
            YELLOW, dpp->biggest_mb, RESET,
            UNIT_MIB, dpp->biggest_kb, UNIT_KIB);
    }
    else if (dpp->biggest_kb > 0) {
        printf(" * %s%lu%s %s\n * %lu bytes\n",
            YELLOW, dpp->biggest_kb, RESET,
            UNIT_KIB, dpp->biggest_bytes);
            
    } else {
        printf(" * %s%lu%s bytes\n",
            YELLOW, dpp->biggest_bytes, RESET);
    }


    printf("\n[%sTiniest%s]\n", GREEN, RESET);
    if (smallest[0]) printf(" - %s\n", smallest);

    if (dpp->smallest_gb > 0) {
        printf(" * %s%lu%s %s\n * %lu %s\n",
            GREEN, dpp->smallest_gb, RESET,
            UNIT_GIB, dpp->smallest_mb, UNIT_MIB);
    }
    else if (dpp->smallest_mb > 0) {
        printf(" * %s%lu%s %s\n * %lu %s\n",
            GREEN, dpp->smallest_mb, RESET,
            UNIT_MIB, dpp->smallest_kb, UNIT_KIB);
    }
    else if (dpp->smallest_kb > 0) {
        printf(" * %s%lu%s %s\n * %lu bytes\n",
            GREEN, dpp->smallest_kb, RESET,
            UNIT_KIB, dpp->smallest_bytes);
            
    } else {
        printf(" * %s%lu%s bytes\n",
            GREEN, dpp->smallest_bytes, RESET);
    }
    

    printf("\nFiles:   %s%lu%s",
        CYAN, dpp->file_count, RESET);
    printf("\nFolders: %s%lu%s\n",
        CYAN, dpp->folder_count, RESET);


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
    static char mode[12];
    
    printf("\nTotal size:");
    if (dpp->total_gb > 0) {
        printf("\n * %lu %s\n * %lu %s\n",
            dpp->total_gb, UNIT_GIB, dpp->total_mb, UNIT_MIB);
    }
    else if (dpp->total_mb > 0) {
        printf("\n * %lu %s\n * %lu %s\n",
            dpp->total_mb, UNIT_MIB, dpp->total_kb, UNIT_KIB);
    }
    else if (dpp->total_kb > 0) {
        printf("\n * %lu %s\n * %lu bytes\n",
            dpp->total_kb, UNIT_KIB, dpp->total_bytes);
    }
    else if (dpp->total_bytes > 0) {
        printf("\n * %lu bytes\n", dpp->total_bytes);
            
    } else {
        printf(" no files or empty file(s)\n");
    }

    
    printf("\nFiles:   %lu", dpp->file_count);
    printf("\nFolders: %lu\n", dpp->folder_count);

    
    snprintf(mode, sizeof(mode), "%s",
        show_blocks ? "*Allocated" : "*Apparent");
    
    if (timer_ms > 0) {
        printf("\n%s (%d ms)\n", mode, timer_ms);
    } else {
        printf("\n%s (< %d ms)\n", mode, timer_ms);
    }
    
    exit(0);
}

