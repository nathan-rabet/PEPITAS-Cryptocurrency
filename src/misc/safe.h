#ifndef SAFE_H
#define SAFE_H

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <errno.h>

/**
 * @brief Write safely to a file descriptor
 * 
 * @param fd The file descriptor
 * @param buf The buffer to write
 * @param count The number of byte to write in fd
 */
int safe_write(int fd, const void *buf, ssize_t count)
{
	ssize_t offset = 0;
	while (count != offset)
	{
		if ((offset += write(fd, buf + offset, count)) == -1)
			return errno;
	}
    return 0;
}
#endif