#ifndef SAFE_H
#define SAFE_H

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/**
 * @brief Write safely to a file descriptor
 * 
 * @param fd The file descriptor
 * @param buf The buffer to write
 * @param count The number of byte to write in fd
 */
int safe_write(int fd, const void *buf, ssize_t count);

/**
 * @brief Read safely in a file descriptor unitl \r\n\r\n
 * 
 * @param fd The file descriptor
 * @param buf The buffer wich contains the message
 * @return The number of byte of buf in fd; -1 for error
 */
ssize_t safe_read(int fd, const void **buf, size_t *bufsize);
#endif