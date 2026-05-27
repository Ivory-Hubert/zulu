/* Zulu - fast & lightweight CLI for directory stats
 * Copyright (C) 2026 Ivori Huobolainen
 * Licensed under the GNU GPL v3 or later
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zulu.h"

volatile int no_colors   = 0;
volatile int is_piped    = 0;
volatile int lite_mode   = 0;
volatile int list_files  = 0;
volatile int byte_list   = 0;
volatile int show_blocks = 0;
volatile int human_sizes = 0;

clock_t beginning = { 0 };


int main(int argc, char **argv) {
    beginning = clock();
    char *cwd = getcwd(NULL, 0);
    const char *flag = argv[1];
    
    const char *zulu_env = getenv("ZULU_NO_COLOR");
    if (zulu_env != NULL && zulu_env[0] != '\0')
        no_colors = 1;
    
    const char *color_env = getenv("NO_COLOR");
    if (color_env != NULL && color_env[0] != '\0')
        no_colors = 1;
        
    if (!isatty(STDOUT_FILENO)) {
        is_piped  = 1;
        no_colors = 1;
    }


    if (argc < 2) {
        printf("zulu> Type 'zulu -H' for usage\n");

        lite_mode = 1;
        searchFolder(0, cwd);
    }
    else if (strcmp(flag, "-a") == 0) {
        printf("zulu> '%s'\n", cwd);
        
        lite_mode   = 1;
        show_blocks = 1;
        
        searchFolder(0, cwd);
    }
    else if (strcmp(flag, "-H") == 0 ||
             strcmp(flag, "--help") == 0) {

        stdout_ui(SHOW_HELP, NULL);
    }
    else if (strcmp(flag, "-V") == 0 ||
             strcmp(flag, "--version") == 0) {

        stdout_ui(SHOW_VER, NULL);
    }
    else if (strcmp(flag, "-l") == 0 ||
             strcmp(flag, "-lh") == 0) {

        listBytes(argc, argv, cwd);
    }
    else if (strcmp(flag, "-s") == 0 ||
             strcmp(flag, "-sa") == 0) {

        summary(argc, argv, cwd);
    }
    else if (strcmp(flag, "-c") == 0) {
        convert(argc, argv);
    }
    else if (strcmp(flag, "-f") == 0) {
        auditFile(argc, argv, cwd);
        
    } else {
        printf("%szulu%s> Unknown '%s'\n", RED, RESET, flag);
    }
    
    return 0;
}

void listBytes(int argc, char **argv, const char *cwd) {
    // zulu -l(h) [PATH]
    int count;
    byte_list = 1;
    human_sizes = (strcmp(argv[1], "-lh") == 0);

    const char *path = argc > 2 ? argv[2] : cwd;

    count = searchFolder(SHOW_LIST_UI, path);
            
    printf("\nCount: %d\n", count);
}

void summary(int argc, char **argv, const char *cwd) {
    // zulu -s(a) [--ls] [PATH]
    const char *path = cwd;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            continue;
        }
        else if (strcmp(argv[i], "-sa") == 0) {
            show_blocks = 1;
        }
        else if (strcmp(argv[i], "--ls") == 0) {
            list_files = 1;
        }
        else if (strcmp(argv[i], "--lsu") == 0) {
            list_files = 1;
            human_sizes = 1;
        }
        else {
            path = argv[i];
        }
    }

    if (path == NULL || strlen(path) == 0) {
        printf("%szulu%s> No directory provided and CWD is unavailable\n", RED, RESET);
        return;
    }
    
    searchFolder(SHOW_SUM_UI, path);
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
    char file_path[4096];
    
    if (argc < 3) {
        printf("%szulu%s> No file or path provided (use <NAME> for a file in CWD)\n", RED, RESET);
        return;
    }
    
    if (argv[2][0] == '/') {
        fileData(argv[2]);
        return;
    }
    
    if (snprintf(file_path, sizeof(file_path), "%s/%s", cwd, argv[2])
        >= (int)sizeof(file_path)) {                
        fprintf(stderr, "zulu> File path too long, terminated.\n");
        exit(1);
    }
    
    fileData(file_path);
}

