#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include "elist.h"
#include "logger.h"

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

struct dir_element
{
    char path[PATH_MAX + 1];
    char fullpath[PATH_MAX + 1];
    off_t size;
    time_t time;
};

int traverse(struct elist *list, DIR *currentDir, char *parentpath, char *parentpath_full) 
{
    struct dirent * ptr;
    struct stat buf;
    char path[PATH_MAX + 1];
    char fullpath[PATH_MAX + 1];
    
    while((ptr = readdir(currentDir)) != NULL)
    {
        struct dir_element *childDir = malloc(sizeof(struct dir_element));
        if (childDir == NULL) {
            perror("cannot malloc child directory struct");
            return -1;
        }

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

        strcpy(childDir->fullpath, fullpath);
        strcpy(childDir->path, path);
        childDir->size = buf.st_size;
        childDir->time = buf.st_atim.tv_sec;

        // log info
        // LOG("file path is %s \n", childDir->fullpath);
        // LOG("file size is %d bytes\n", childDir->size);
        // LOG("file mode is %i \n", S_ISDIR(buf.st_mode));
        // LOG("file time is %d\n", childDir->time);

        if (S_ISDIR(buf.st_mode) && strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
            DIR *dir = opendir(fullpath);
            if (dir == NULL) {
                fprintf(stderr, "Unable to open directory: [%s]\n", fullpath);
                return -1;
            }
            int res = traverse(list, dir, path, fullpath);
            if (res == -1) {
                perror("child traversal error");
                continue;
            }
        }
        elist_add(list, childDir);
        free(childDir);
    }
    closedir(currentDir);
    return 0;
}

int compareTime(const void *o1, const void *o2) {
    time_t t1 = ((struct dir_element *)o1)->time;
    time_t t2 = ((struct dir_element *)o2)->time;
    if (t1 == t2) {
        return 0;
    } else {
        return t1 < t2 ? 1: -1;
    }
}

int compareSize(const void *o1, const void *o2) {
    off_t s1 = ((struct dir_element *)o1)->size;
    off_t s2 = ((struct dir_element *)o2)->size;
    if (s1 == s2) {
        return 0;
    } else {
        return s1 < s2 ? -1 : 1;
    }
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
    
    // TODO: traverse the directory and store entries in the list
    ssize_t traverseRes = traverse(dirList, dir, &options.directory, fullpath);

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

    // TODO: print formatted list
    LOGP("checking the list \n");
    LOG("List size is: %d\n", elist_size(dirList));
    for (int idx = 0; idx < elist_size(dirList); idx++) {
        struct dir_element *elem = elist_get(dirList, idx);
        char time_buff[16];
        time_t rawtm = elem->time;
        LOG("raw: %d\n", rawtm);

        struct tm *ltime = localtime(&rawtm);
        strftime(time_buff, 16, "    %b %d %Y", ltime);
        LOG("%s\n", elem->fullpath);
        LOG("last acc: %s\n", time_buff);
        LOG("sz: %d\n", elem->size);
    }

    elist_destroy(dirList);

    return 0;
}
