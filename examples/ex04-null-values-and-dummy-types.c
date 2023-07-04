#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "i_writer.h"
#include "iic.h"

typedef int fd_writer;

void fd_writer_write(void *_fdw, const char *str)
{
	fd_writer *fd = _fdw;
	write(*fd, str, strlen(str));
}

void fd_writer_close(void *_fdw)
{
	fd_writer *fd = _fdw;
	close(*fd);
}

typedef struct chat_writer
{
	i_writer writer;
	const char *name;
} chat_writer;

void chat_writer_write(void *_cw, const char *str)
{
	chat_writer *cw = _cw;
	i_writer writer = cw->writer;
	writer.write(writer.o, cw->name);
	writer.write(writer.o, ": ");
	writer.write(writer.o, str);
	writer.write(writer.o, "\n");
}

void chat_writer_close(void *_cw)
{
	chat_writer *cw = _cw;
	i_writer writer = cw->writer;
	writer.close(writer.o);
}

int main(void)
{
	fd_writer fdw = 1;
	i_writer writer = i_writer_new(&fdw, fd_writer);
	chat_writer cw = {.writer = writer, .name = "Legolas"};
	i_writer writer2 = i_writer_new(&cw, chat_writer);

	fd_writer *fdw_casted;
	chat_writer *cw_casted;

	if (fdw_casted = iic_dyncast(fd_writer, writer))
	{
		printf("writer cast to fd_writer successful\n");
		fd_writer_write(fdw_casted, "Using fd_writer directly\n");
	}
	else if (cw_casted = iic_dyncast(chat_writer, writer))
	{
		printf("writer cast to chat_writer successful\n");
		chat_writer_write(cw_casted, "Using chat_writer directly");
	}

	if (fdw_casted = iic_dyncast(fd_writer, writer2))
	{
		printf("writer2 cast to fd_writer successful\n");
		fd_writer_write(fdw_casted, "Using fd_writer directly\n");
	}
	else if (cw_casted = iic_dyncast(chat_writer, writer2))
	{
		printf("writer2 cast to chat_writer successful\n");
		chat_writer_write(cw_casted, "Using chat_writer directly");
	}
}
