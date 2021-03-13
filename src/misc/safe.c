#include "safe.h"

int safe_write(int fd, const void *buf, ssize_t count)
{
	ssize_t offset = 0;
	while (count > 0)
	{
		offset = write(fd, buf, count);
		buf += offset;
		count -= offset;
		if (offset == -1)
			return errno;
	}
    return 0;
}


int safe_read(int fd, const void **buf, size_t *bufsize)
{
	size_t buffersize = 256;
	size_t read_count = 0;
	char* buffer = calloc(buffersize, sizeof(char));
	do
	{
		ssize_t nb_read = read(fd, buffer + read_count, buffersize - read_count);
		if (nb_read == -1)
			return -1;
		read_count += nb_read;
		if (read_count >= buffersize)
		{
			buffersize *= 2;
			buffer = realloc(buffer, buffersize);
		}
	} while (strncmp("\r\n\r\n", buffer + (read_count - 5), 4) != 0);
	*buf = buffer;
	*bufsize = buffersize;
	return read_count;
}