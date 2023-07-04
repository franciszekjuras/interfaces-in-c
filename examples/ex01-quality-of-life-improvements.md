This example shows similar functionality as the proof of concept.
However, this time it uses 'interfaces in C' macros and code generation
to reduce boiler-plate.
```c
#include <unistd.h>
#include <string.h>
```
`i_writer.h` is generated from `writer.itf.c` using `iicgen.sh` script:
```sh
../iicgen.sh interfaces/writer.iic.c
```
`writer.iic.c` file contains interface definition in the form of function prototypes:
```c
void write(void *writer_o, const char *str);
void close(void *writer_o);
```
To make the example less trivial, a `close` function was added to the interface.

> See iicgen.sh source for more information about code generation
```c
#include "i_writer.h"

```
No changes in this part
```c

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

void hello_world(i_writer writer)
{
	writer.write(writer.o, "hello world\n");
}

```
`i_writer.h` provides the `i_writer` type and the `i_writer_new` macro
that takes an object pointer and a concrete type name and returns
a properly initialized `i_writer` object. No manual assignment of function
pointers required.
> Compilation will fail if the object pointer type doesn't match
> the specified type name.
```c
int main(void)
{
	fd_writer fdw = dup(1);
	i_writer writer = i_writer_new(&fdw, fd_writer);
	hello_world(i_writer_new(&fdw, fd_writer));

	chat_writer cw = {.writer = writer, .name = "Legolas"};
	i_writer writer2 = i_writer_new(&cw, chat_writer);
	// i_writer writer_fail = i_writer_new(&cw, fd_writer); // does not compile
	writer2.write(writer2.o, "They taking the hobbits to Isengard!");
	writer2.close(writer2.o);
}
```
