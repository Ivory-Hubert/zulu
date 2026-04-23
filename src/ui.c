/* Zulu - fast & lightweight CLI for directory stats
 * Copyright (C) 2026 Ivori Huobolainen
 * Licensed under the GNU GPL v3 or later
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zulu.h"


void showHelp() {
    printf("Zulu usage: zulu [-flag] [optional arg] <required arg>\n\n");
    
    printf("    -s [--ls]         Display summary of current directory\n");
    printf("    -p [--ls] <PATH>  Display summary of chosen directory\n");
    printf("    -b [PATH]         List the current or chosen directory with byte sizes\n");
    printf("    -c <bytes>        Convert provided bytes and display results\n");
    printf("    -f <PATH>         Display detailed file data, in CWD or provided file path\n");
    printf("    -h                Display this help message and exit\n");
    printf("    -v                Display version number and exit\n");

    printf("\nOptional '--ls' flag will also list the files that Zulu counts.\n");
    printf("\nIf no flags are provided Zulu will display the bare minimum, without colors.\n");
    printf("\n");
}

void showVer() {
    printf("zulu - 0.1.1\n\n");
    printf("(C) 2026 Ivori Huobolainen\n");
    printf("Licensed under the GNU GPL v3 or later\n<https://www.gnu.org/licenses/gpl-3.0.html>\n");
}

void display(struct displayParam *dpp, int timer_ms) {
    if (list_files) {
        printf("\n==== %sFolder data%s ====\n",
            cyan, reset);
    }

    printf("\n%sTotal%s:", cyan, reset);
    if (dpp->total_gb > 0) {
        printf("\n * %s%lu%s GiB\n * %s%lu%s MiB\n",
            cyan, dpp->total_gb, reset,
            cyan, dpp->total_mb, reset);
    }
    else if (dpp->total_bytes >= 1) {
        printf("\n * %s%lu%s bytes\n\n",
            cyan, dpp->total_bytes, reset);
    }
    else if (dpp->total_kb > 0) {
        printf("\n * %s%lu%s MiB\n * %s%lu%s KiB\n",
            cyan, dpp->total_mb, reset,
            cyan, dpp->total_kb, reset);
    }
    else {
        printf(" empty folder or empty file(s)\n\n");
        return;
    }

    
    printf("\n%sBiggest%s:\n", yellow, reset);
    if (biggest[0]) printf(" - %s\n", biggest);
    
    if (dpp->biggest_gb > 0) {
        printf(" * %s%lu%s GiB\n * %s%lu%s MiB\n",
            yellow, dpp->biggest_gb, reset,
            yellow, dpp->biggest_mb, reset);
    }
    else if (dpp->biggest_bytes > 0) {
        printf(" * %s%lu%s bytes\n\n",
            yellow, dpp->biggest_bytes, reset);
    }
    else {
        printf(" * %s%lu%s MiB\n * %s%lu%s KiB\n",
            yellow, dpp->biggest_mb, reset,
            yellow, dpp->biggest_kb, reset);
    }

    
    printf("\n%sSmallest%s:\n", green, reset);
    if (smallest[0]) printf(" - %s\n", smallest);
    
    if (dpp->smallest_gb > 0) {
        printf(" * %s%lu%s GiB\n * %s%lu%s MiB\n",
            green, dpp->smallest_gb, reset,
            green, dpp->smallest_mb, reset);
    }
    else if (dpp->smallest_kb == 0) {
        printf(" * %s%lu%s bytes\n",
            green, dpp->smallest_bytes, reset);
    }
    else {
        printf(" * %s%lu%s MiB\n * %s%lu%s KiB\n",
            green, dpp->smallest_mb, reset,
            green, dpp->smallest_kb, reset);
    }


    printf("\nCount: %s%lu%s\n",
        cyan, dpp->file_count, reset);
    
    
    if (timer_ms > 0) {
        printf("\n(Took %s%d%s ms)\n",
            cyan, timer_ms, reset);
    } else {
        printf("\n(Took < %s%d%s ms)\n",
            cyan, timer_ms, reset);
    }
    
    exit(0);
}

void liteDisplay(struct displayParam *dpp, int timer_ms) {
    printf("\nFile count: %lu\n", dpp->file_count);

    printf("\nTotal size:");
    if (dpp->total_gb > 0) {
        printf(" %lu GiB\n", dpp->total_gb);
    }
    else if (dpp->total_mb > 0) {
        printf(" %lu MiB\n", dpp->total_mb);
    }
    else if (dpp->total_kb > 0) {
        printf(" %lu KiB\n", dpp->total_kb);
    }
    else if (dpp->total_bytes > 0) {
        printf(" %lu bytes\n", dpp->total_bytes);
    }
    else {
        printf(" empty directory or empty file(s)\n");
    }

    
    if (timer_ms > 0) {
        printf("\n(Took %d ms)\n", timer_ms);
    } else {
        printf("\n(Took < %d ms)\n", timer_ms);
    }
    
    exit(0);
}

