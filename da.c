#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#include "elist.h"
#include "logger.h"

#define TIME_COLS 15
#define SIZE_COLS 14
#define TRIAL_SPACE_COLS 13

/* Forward declarations: */
void print_usage(char *argv[]);


void print_usage(char *argv[]) {
    fprintf(stderr, "Disk Analyzer (da): analyzes disk space usage\n");
    fprintf(stderr, "Usage: %s [-ahs] [-l limit] [directory]\n\n", argv[0]);

    fprintf(stderr, "If no directory is specified, the current working directory is used.\n\n");

    fprintf(stderr, "Options:\n"
    "    * -a              Sort the files by time of last access (descending)\n"
    "    * -h              Display help/usage information\n"
    "    * -l limit        Limit the output to top N files (default=unlimited)\n"
    "    * -s              Sort the files by size (default, ascending)\n\n"
    );
}


/** @struct dir_element
 *  @brief This structure is used to store the temperary information of file and dirctory for traverse and print out the file and directoy  
 */
struct dir_element
{
    char path[PATH_MAX + 1];        /*!< Relative path to display to users */
    char fullpath[PATH_MAX + 1];    /*!< Full path used to open direcotry and access stats */
    off_t size;                     /*!< Size of the file or direcotry in bytes */
    time_t time;                    /*!< Last access time of file or directory in msec */
};

/**
 * Helper function that traverses the dictories recursively
 *
 * @param list The list to store files and directories
 * @param currentDir the directory to be traversed at the current call stack level
 * @param parentpath the relative path of the parent directory, used to construct the display name (dir_element.path)
 * @param parentpath_full the full path of the parent directory, used to construct the fullpath
 * @return zero on success, nonzero on failure
 */
int traverse(struct elist *list, DIR *currentDir, char *parentpath, char *parentpath_full) 
{
    struct dirent * ptr;
    struct stat buf;
    char path[PATH_MAX + 1];
    char fullpath[PATH_MAX + 1];
    
    while((ptr = readdir(currentDir)) != NULL)
    {

        // LOG("Checking dir: %s\n", ptr->d_name);
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
            strcpy(path, parentpath);
            strcat(path, "/");
            strcat(path, ptr->d_name);

            strcpy(fullpath, parentpath_full);
            strcat(fullpath, "/");
            strcat(fullpath, ptr->d_name);
        } else {
            // already checked the current directory and parent directory if is not root
            continue;
        }
        
        if(path == NULL || fullpath == NULL) {
               perror("path");
               return -1;
        }
        if (stat(fullpath, &buf) == -1) {
               perror("stat");
               break;
               return -1;
        }

        struct dir_element childDir = {"","", buf.st_size, buf.st_atim.tv_sec};

        strcpy(childDir.fullpath, fullpath);
        strcpy(childDir.path, path);

        if (S_ISDIR(buf.st_mode) && strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
            DIR *dir = opendir(fullpath);
            if (dir == NULL) {
                fprintf(stderr, "Unable to open directory: [%s]\n", fullpath);
                return -1;
            }
            int res = traverse(list, dir, path, fullpath);
            closedir(dir);
            
            if (res == -1) {
                perror("child traversal error");
                continue;
            }
        } else {
            elist_add(list, &childDir);
        }
    }
    return 0;
}

/**
 * Comparator that compares the last access time of the dir_element
 *
 * @param o1 The dir_element ptr, pointing to one of the dir_element to be compared
 * @param o2 The dir_element ptr, pointing to the other of the dir_element to be compared
 * @return zero on same, negative on t1 < t2, positive on t1 > t2 
 */
int compareTime(const void *o1, const void *o2) {
    time_t t1 = ((struct dir_element *)o1)->time;
    time_t t2 = ((struct dir_element *)o2)->time;
    if (t1 == t2) {
        return 0;
    } else {
        return t1 < t2 ? -1: 1;
    }
}

/**
 * Comparator that compares the size of the dir_element
 *
 * @param o1 The dir_element ptr, pointing to one of the dir_element to be compared
 * @param o2 The dir_element ptr, pointing to the other of the dir_element to be compared
 * @return zero on same, negative on s1 > t2, positive on s1 < s2 
 */
int compareSize(const void *o1, const void *o2) {
    off_t s1 = ((struct dir_element *)o1)->size;
    off_t s2 = ((struct dir_element *)o2)->size;
    if (s1 == s2) {
        return 0;
    } else {
        return s1 < s2 ? 1 : -1;
    }
}

/**
 * Compute the size of the window
 * 
 * @return the window col number, if not able to get, return a default value of 80 
 */
unsigned short calColumn(void) {
    unsigned short cols = 80;
    struct winsize win_sz;
    if (ioctl(fileno(stdout), TIOCGWINSZ, &win_sz) != -1) {
        cols = win_sz.ws_col;
    }
    LOG("Display columns: %d\n", cols);
    return cols;
}

/**
 * Helper method to convert size into human readable size string
 *
 * @param buf The string to store the human readable size
 * @param max The max length of the buf string
 * @param sz The size need to be converted
 * @return zero on success, non-zero on failure
 */
ssize_t convert_size( char *buf, unsigned int max, off_t sz) {
    const char * units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB"};
    size_t len = sizeof(units)/sizeof(char *);
    size_t unitIdx = 0;
    double size = (double) sz;
    while (size > 1024 && unitIdx < len) {
        size /= 1024;
        unitIdx++;
    }
    if (unitIdx == len) {
        perror("file too large");
        return 1;
    } 
    ssize_t res = snprintf(buf, max + 1, "    %.1f %s", size, units[unitIdx]);
    if (res == -1) {
        perror("unable to convert size");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    /* Create a struct to hold program options and initialize it by declaring an
     * 'options' variable. Defaults:
     *      - sort by size (time=false)
     *      - limit of 0 (unlimited)
     *      - directory = '.' (current directory) */
    struct da_options {
        bool sort_by_time;
        unsigned int limit;
        char *directory;
    } options
        = { false, 0, "." };

    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "ahl:s")) != -1) {
        switch (c) {
            case 'a':
                options.sort_by_time = true;
                break;
            case 'h':
                print_usage(argv);
                return 0;
            case 's':
                options.sort_by_time = false;
                break;
            case 'l': {
                /*    ^-- to declare 'endptr' here we need to enclose this case
                 *    in its own scope with curly braces { } */
                char *endptr;
                long llimit = strtol(optarg, &endptr, 10);
                if (llimit < 0 || llimit > INT_MAX || endptr == optarg) {
                    fprintf(stderr, "Invalid limit: %s\n", optarg);
                    print_usage(argv);
                    return 1;
                }
                options.limit = (int) llimit;
                break;
                }
            case '?':
                if (optopt == 'l') {
                    fprintf(stderr,
                            "Option -%c requires an argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n", optopt);
                }
                print_usage(argv);
                return 1;
            default:
                abort();
        }
    }

    if (optind < argc) {
        options.directory = argv[optind];
    }

    LOGP("Done parsing arguments.\n");
    LOG("Sorting by: [%s], limit: [%u]\n",
            options.sort_by_time == true ? "time" : "size",
            options.limit);
    LOG("Directory to analyze: [%s]\n", options.directory);
    char fullpath[PATH_MAX];
    realpath(options.directory, fullpath);
    LOG("Fullpath of dir: [%s]\n", fullpath);

    DIR *dir = opendir(options.directory);
    if (dir == NULL) {
        fprintf(stderr, "Unable to open directory: [%s]\n", options.directory);
        return -1;
    }

    struct elist *dirList = elist_create(0, sizeof(struct dir_element));
    if (dirList != NULL) {
        LOGP("Created an elist to hold dirs. \n");
    } else {
        fprintf(stderr, "Unable to create the list.\n");
        return -1;
    }
    
    char display_path[PATH_MAX];
    if (options.directory[0] != '.') {
        strcpy(display_path, "./");
        strcat(display_path, options.directory);
    } else {
        strcpy(display_path, options.directory);
    }
    // traverse
    ssize_t traverseRes = traverse(dirList, dir, display_path, fullpath);
    closedir(dir);

    if (traverseRes != 0) {
        perror("traversal failed");
        elist_destroy(dirList);
        return -1;
    }

    if (options.sort_by_time) {
        elist_sort(dirList, compareTime);
    } else {
        elist_sort(dirList, compareSize);
    }

    LOGP("checking the list \n");
    LOG("List size is: %zu\n", elist_size(dirList));

    int max = elist_size(dirList);
    if (options.limit != 0) { 
        max = max > options.limit ? options.limit : max; 
    } 

    unsigned short totalCols = calColumn();
    // -2 for holding \n and \0
    unsigned short nameCols = totalCols - TIME_COLS - SIZE_COLS;
    LOG("total col: %d\n", totalCols);

    for (int idx = 0; idx < max; idx++) {
        struct dir_element *elem = elist_get(dirList, idx);
        char name_buff[nameCols + 1];
        char time_buff[TIME_COLS + 1];
        char size_buff[SIZE_COLS + 1];
        time_t rawtm = elem->time;

        struct tm *ltime = localtime(&rawtm);

        strftime(time_buff, TIME_COLS + 1, "    %b %d %Y", ltime);

        if (convert_size(size_buff, SIZE_COLS + 1, elem->size) != 0) {
            strcpy(size_buff, "    EXCEED");
        }
        
        if (strlen(elem->path) > nameCols) {
            int startIdx = strlen(elem->path) - nameCols + 3;
            char tmp[nameCols + 1];
            strcpy(tmp, "...");
            strcat(tmp, elem->path+startIdx);
            strcpy(elem->path, tmp);
        }
        snprintf(name_buff, nameCols + 1, "%s", elem->path);

        printf("%*s%*s%*s\n", nameCols, name_buff, SIZE_COLS, size_buff, TIME_COLS, time_buff);
    }

    elist_destroy(dirList);

    return 0;
}
