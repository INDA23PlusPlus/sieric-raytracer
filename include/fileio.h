#ifndef FILEIO_H_
#define FILEIO_H_

#include <sys/types.h>

const char *data_path(const char *);
ssize_t data_file_to_buf(char **, const char *);

#endif /* FILEIO_H_ */
