 This is the initial proof of concept for 'interfaces in C'.
```c

#include <unistd.h>
#include <string.h>

```
 `i_writer` struct is an interface struct.
 `void *o` pointer stores pointer of unknown type.
 `write` is a function pointer - the function it points to
 should take `o` pointer as the first argument and cast it to
 the concrete type.
```c
typedef struct i_writer
{
	void *o;
	void (*write)(void *, const char *);
} i_writer;

```
 Two concrete types are defined: `fd_writer` and `chat_writer`.
 Both implement a `write` function - to prevent naming collision appropriate type names
 are prepended to these functions.
 There are no constraints on concrete types.
```c

typedef int fd_writer;

```
 `fd_writer_write` function writes the `str` string to a file descriptor.
```c
void fd_writer_write(void *_fdw, const char *str)
{
	fd_writer *fd = _fdw;
	write(*fd, str, strlen(str));
}

```
 Note that `chat_writer` embeds an i_writer interface.
```c
typedef struct chat_writer
{
	i_writer writer;
	const char *name;
} chat_writer;

```
 `chat_writer_write` writes the `str` string prepended with `name` using
 an `i_writer` interface.
```c
void chat_writer_write(void *_cw, const char *str)
{
	chat_writer *cw = _cw;
	i_writer writer = cw->writer;
	writer.write(writer.o, cw->name);
	writer.write(writer.o, ": ");
	writer.write(writer.o, str);
	writer.write(writer.o, "\n");
}

```
 `hello_world` function is a trivial example of passing interface
 by value and how to call an interface function.
```c
void hello_world(i_writer writer)
{
	writer.write(writer.o, "hello world\n");
}

```
 In the first part of the `main` function an `i_writer` interface is created
 from a `fd_writer` instance and the `fd_writer_write` function
 and it's passed to the `hello_world` function.
 In the second part, a `chat_writer` object is created with embedded
 `i_writer` instance and the `chat_writer_write` function is called directly
```c
int main(void)
{
	fd_writer fdw = 1;
	i_writer writer = {.o = &fdw, .write = fd_writer_write};
	hello_world(writer);

	chat_writer cw = {.writer = writer, .name = "Legolas"};
	chat_writer_write(&cw, "They taking the hobbits to Isengard!");
}
```
