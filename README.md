# Boba: A mediocre redis clone

![Boba](http://raw.github.com/Najaf/boba/master/boba.jpg)

## Why why why?

Purely for educational purposes.

In no particular order here are the things I'm trying to learn:

* **Heap memory allocation**
* **Data Structures**. In particular:
  * Linked Lists
  * Dynamic Arrays
  * Hash functions
  * Trees
  * Graph data structures
* **Networking at the TCP/IP layer**
* **I/O, including special files**
* **Managing larger C codebases with make and friends**
* **Writing and using C libraries**
* **Debugging with tools like valgrind and gdb**
* **Programming with OS threads + processes**

## Building

```bash
$ cd boba/src
$ make
```

Run `make clean` before commiting anything.

## Usage

To run on port 4567, in one terminal:

```bash
$ ./boba-server 4567
Listening for connections on port 4567...
```

in another:
```bash
$ ./boba-client 127.0.0.1 4567
Connected to 127.0.0.1 on port 4567...
```

You should see the following output in the server:

```
Accepting inbound TCP connection
```

For now the client just sends messages and the server outputs the message.

```bash
Client:

> hello, world!

Server:

Read data (13 bytes): hello, world!
```

To exit the client:

```bash
> exit
```

## Todo

* Create an `echo` command that just sends back whatever data the client sent
* Start parsing messages into commands for the server
* Implement a hash table that the client can add and remove objects from
* Write more stuff to do
