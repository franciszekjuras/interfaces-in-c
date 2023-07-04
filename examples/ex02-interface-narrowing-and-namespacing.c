#include <unistd.h>
#include <string.h>
#include "i_writer.h"
/**
`i_readwriter` interfaces extends (implicitly) `i_writer` with `read` function.
 */
#include "i_readwriter.h"

typedef struct fd_readwriter
{
	int read;
	int write;
} fd_readwriter;

/**
This is `write` implementation for `fd_readwriter` type.
Note shorter `fd_rw` prefix.
 */
void fd_rw_write(void *_fdrw, const char *str)
{
	fd_readwriter *fds = _fdrw;
	write(fds->write, str, strlen(str));
}

/**
Dummy `read` function
 */
const char *fd_rw_read(void *_fdrw)
{
	(void)_fdrw;
	return NULL;
}

/**
 */
void fd_rw_close(void *_fdrw)
{
	fd_readwriter *fds = _fdrw;
	close(fds->read);
	close(fds->write);
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

void hello_world(i_writer writer)
{
	writer.write(writer.o, "hello world\n");
}

int main(void)
{
	fd_readwriter fdw = {.read = dup(0), .write = dup(1)};
	/**
`i_readwriter_new_ns` macro allows specifying custom function prefix.
	 */
	i_readwriter readwriter = i_readwriter_new_ns(&fdw, fd_readwriter, fd_rw);
	/**
With shorter prefix calling concrete implementations directly requires less typing.
	 */
	fd_rw_write(&fdw, "Hello darkness my old friend...\n");
	/**
`hello_world` function and `chat_writer` struct require `i_writer` type.
Because `i_readwriter` implements all functions	required by `i_writer`,
we can downcast using `i_writer_from`. Specifying type or namespace
is not necessary.
	 */
	hello_world(i_writer_from(readwriter));
	chat_writer cw = {.writer = i_writer_from(readwriter), .name = "Legolas"};
	i_writer writer = i_writer_new(&cw, chat_writer);
	writer.write(writer.o, "They taking the hobbits to Isengard!");
	writer.close(writer.o);
}
