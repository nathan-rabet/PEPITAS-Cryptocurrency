#ifndef SAFE_H
#define SAFE_H

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/**
 * @brief Writes safely to a file descriptor
 * 
 * @param fd The file descriptor
 * @param buf The buffer to write
 * @param count The number of byte to write in fd
 * 
 * @return Error code
 */
int safe_write(int fd, const void *buf, ssize_t count);

/**
 * @brief Reads safely in a file descriptor until '\\r\\n\\r\\n'
 * 
 * @param fd The file descriptor
 * @param buf The buffer which contains the message
 * @return The number of byte the file 'fd', if -1 error
 */
ssize_t safe_read(int fd, const void **buf, size_t *bufsize);
#endif