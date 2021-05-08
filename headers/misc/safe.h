#ifndef SAFE_H
#define SAFE_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

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
 * @brief Send safely to a file descriptor
 * 
 * @param fd The file descriptor
 * @param buf The buffer to write
 * @param count The number of byte to write in fd
 * 
 * @return Error code
 */
int safe_send(int fd, const void *buf, ssize_t count);

/**
 * @brief Reads safely in a file descriptor until '\\r\\n\\r\\n'
 * 
 * @param fd The file descriptor
 * @param buf The buffer which contains the message
 * @return The number of byte the file 'fd', if -1 error
 */
ssize_t safe_read(int fd, const void **buf, size_t *bufsize);

/**
 * @brief Calls 'fread' but safely !
 * 
 * @param buffer The buffer to write on
 * @param size The size of 1 read element
 * @param n The number of elements to read
 * @param file The IO FILE
 * @return ssize_t, -1 if error  or the number of read items
 */
ssize_t safe_fread(void *buffer, const size_t size, const size_t n, FILE *file);
#endif