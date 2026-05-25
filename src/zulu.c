/* Zulu - fast & lightweight CLI for directory stats
 * Copyright (C) 2026 Ivori Huobolainen
 * Licensed under the GNU GPL v3 or later
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zulu.h"

volatile int no_colors = 0;
volatile int is_piped = 0;
volatile int lite_mode = 0;
volatile int list_files = 0;
volatile int byte_list = 0;
volatile int show_blocks = 0;
volatile int human_sizes = 0;

clock_t beginning = { 0 };


int main(int argc, char **argv) {
    beginning = clock();
    char *cwd = getcwd(NULL, 0);

    const char *zulu_env = getenv("ZULU_NO_COLOR");
    if (zulu_env != NULL && zulu_env[0] != '\0')
        no_colors = 1;
    
    const char *color_env = getenv("NO_COLOR");
    if (color_env != NULL && color_env[0] != '\0')
        no_colors = 1;
        
    if (!isatty(STDOUT_FILENO)) {
        is_piped = 1;
        no_colors = 1;
    }


    if (argc < 2) {
        printf("zulu> Type 'zulu -h' for help\n");

        lite_mode = 1;
        searchFolder(cwd);
    }
    else if (strcmp(argv[1], "-a") == 0) {
        printf("zulu> '%s'\n", cwd);
        
        lite_mode = 1;
        show_blocks = 1;
        searchFolder(cwd);
    }
    else if (strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {

        stdout_ui(SHOW_HELP);
    }
    else if (strcmp(argv[1], "-v") == 0) {
        stdout_ui(SHOW_VER);
    }
    else if (strcmp(argv[1], "-l") == 0 ||
        strcmp(argv[1], "-lh") == 0) {
        listBytes(argc, argv, cwd);
    }
    else if (strcmp(argv[1], "-s") == 0 ||
        strcmp(argv[1], "-sa") == 0) {

        listAll(argc, argv, cwd);
    }
    else if (strcmp(argv[1], "-p") == 0 ||
        strcmp(argv[1], "-pa") == 0) {

        listPath(argc, argv);
    }
    else if (strcmp(argv[1], "-c") == 0) {
        convert(argc, argv);
    }
    else if (strcmp(argv[1], "-f") == 0) {
        auditFile(argc, argv, cwd);
        
    } else {
        printf("%szulu%s> Unknown parameter '%s'\n", RED, RESET, argv[1]);
    }
    
    return 0;
}

void listBytes(int argc, char **argv, const char *cwd) {
    // zulu -b [PATH]
    byte_list = 1;
    human_sizes = (strcmp(argv[1], "-lh") == 0);

    printf("Listing: %s\n\n", (argc > 2 ? argv[2] : cwd));

    if (!is_piped) {
        printf(" %-34s | Sizes\n", "  File");
        printf(" %-34s |------\n", "------");
    }

    int count = searchFolder(argc > 2 ? argv[2] : cwd);
            
    printf("\nCount: %d\n", count);
}

void listAll(int argc, char **argv, const char *cwd) {
    // zulu -s [--ls]
    printf("%szulu%s> '%s'\n", CYAN, RESET, cwd);

    show_blocks = (strcmp(argv[1], "-sa") == 0);
    
    if (argc > 2 && strcmp(argv[2], "--ls") == 0) {
        list_files = 1;
        printf("\n====== %sList%s ======\n", CYAN, RESET);
    }
        
    searchFolder(cwd);
}

void listPath(int argc, char **argv) {
    // zulu -p [--ls] <PATH>
    if (argc < 3) {
        printf("%szulu%s> No directory provided\n", RED, RESET);
        return;
    }

    show_blocks = (strcmp(argv[1], "-pa") == 0);
    
    printf("%szulu%s> '%s'\n",
        CYAN, RESET,
        (argc > 3 ? argv[3] : argv[2]));

    if (argc > 3 && strcmp(argv[2], "--ls") == 0) {
        list_files = 1;

        printf("\n====== %sList%s ======\n", CYAN, RESET);

        searchFolder(argv[3]);
    }

    searchFolder(argv[2]);
}

void convert(int argc, char **argv) {
    // zulu -c <bytes>
    if (argc < 3) {
        printf("%szulu%s> No byte size provided\n", RED, RESET);
        return;
    }

    byteMath(argv[2]);
}

void auditFile(int argc, char **argv, const char *cwd) {
    // zulu -f <PATH>
    if (argc < 3) {
        printf("%szulu%s> No file or path provided (use <NAME> for a file in CWD)\n", RED, RESET);
        return;
    }
    
    if (argv[2][0] == '/') {
        fileData(argv[2]);
        return;
    }

    char file_path[4096];
    
    if (snprintf(file_path, sizeof(file_path), "%s/%s", cwd, argv[2])
        >= (int)sizeof(file_path)) {                
        fprintf(stderr, "zulu> File path too long, terminated.\n");
        exit(1);
    }
    
    fileData(file_path);
}

