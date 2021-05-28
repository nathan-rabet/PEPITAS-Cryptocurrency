#include "misc/safe.h"

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

int safe_send(int fd, const void *buf, ssize_t count)
{
	ssize_t offset = 0;
	while (count > 0)
	{
		offset = send(fd, buf, count, MSG_MORE);
		buf += offset;
		count -= offset;
		if (offset == -1)
			return errno;
	}
	return 0;
}

ssize_t safe_read(int fd, const void **buf, size_t *bufsize)
{
	size_t buffersize = 12;
	size_t read_count = 0;
	char *buffer = calloc(buffersize, sizeof(char));
	do
	{
		ssize_t nb_read = read(fd, buffer + read_count, buffersize - read_count);
		if (nb_read == 0)
		{
			break;
		}
		
		if (nb_read == -1)
			return -1;
		read_count += nb_read;
		if (read_count >= buffersize)
		{
			buffersize += 1;
			buffer = realloc(buffer, buffersize);
		}
	} while (strncmp("\r\n\r\n", buffer + (read_count - 4), 4));
	*buf = buffer;
	*bufsize = buffersize;
	return read_count;
}

ssize_t safe_fread(void *buffer, const size_t size, const size_t n, FILE *file)
{
	size_t total_count = 0;
	ssize_t nb_read = 0;
	do
	{
		nb_read = fread(buffer + (total_count * size), size, n - total_count, file);
		if (nb_read == -1)
			return -1;
		total_count += nb_read;
	} while (nb_read > 0);
	return total_count;
}