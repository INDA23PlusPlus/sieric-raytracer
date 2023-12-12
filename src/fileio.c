#include <fileio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define DATA_PATH_OFF "../share/tayracer/"

const char *data_path(const char *path) {
    static char real[1024] = DATA_PATH_OFF;
    strncpy(real + sizeof DATA_PATH_OFF - 1, path,
            sizeof real - sizeof DATA_PATH_OFF - 1);
    return real;
}

ssize_t data_file_to_buf(char **buf, const char *path) {
    FILE *f;
    ssize_t sz;

    const char *new = data_path(path);
    if(!(f = fopen(new, "r"))) return -1;

    fseek(f, 0, SEEK_END);
    sz = ftell(f);
    rewind(f);

    *buf = malloc(sz + 1);
    fread(*buf, sizeof(char), sz, f);
    (*buf)[sz] = '\0';
    fclose(f);
    return sz + 1;
}
