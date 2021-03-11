#include "util.h"

#include <stdio.h>
#include <time.h>

void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimals)
{
    const char * units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB"};
    size_t len = sizeof(units)/sizeof(char *);
    size_t unitIdx = 0;
    while (size > 1024 && unitIdx < len) {
        size /= 1024;
        unitIdx++;
    }
    if (unitIdx == len) {
        perror("file too large");
    } 
    ssize_t res = snprintf(buf, buf_sz, "    %.*f %s", decimals, size, units[unitIdx]);
    if (res == -1) {
        perror("unable to convert size");
    }
}

size_t simple_time_format(char *buf, size_t buf_sz, time_t time)
{
    struct tm *ltime = localtime(&time);

    return strftime(buf, buf_sz, "    %b %d %Y", ltime);
}
