# I/O utilities for KDB

## Overview

This module provides extended I/O functionality for **KDB**, including file system operations, file locking, and async network connections.

## Import

You can load the module like

```q
.q,:use`dot.io
```

That will add the functions to the `.q` namespace that allows using infix notations, like for any other keywords.

If you don't like that, you can add them to the namespace of your liking, eg. to add them to `.io`, you can

```q
.io:use`dot.io
```

The rest of this document assumes the `.q` namespace.

## The API

### stat

Get file or directory information via posix `stat`.

```q
stat x
```

where `x` can be:
* A file path symbol (`:path` or `path`) or string
* A file handle (int/long)

Returns a dictionary with:
* `type` - file type (`file`, `directory`, `symlink`, `socket`, etc.)
* `size` - file size in bytes
* `links` - number of hard links
* `mode` - permissions as a vector of three shorts
* `inode` - inode number
* `device` - device ID
* `blksize` - block size
* `blocks` - number of blocks allocated
* `change` - last status change timestamp
* `modify` - last modification timestamp
* `access` - last access timestamp
* `uid` - owner user ID
* `gid` - owner group ID

### flags

Get file descriptor flags.

```q
flags x
```

where `x` is a file handle (int/long).

Returns a symbol vector containing applicable flags:
* `read` - file is readable
* `write` - file is writable
* `append` - file is opened in append mode
* `nonblock` - file is in non-blocking mode
* `sync` - synchronous writes
* `dsync` - data-only synchronous writes

### info

Get detailed socket or file information.

```q
hinfo x
```

where `x` is a file handle (int/long).

For sockets, returns a dictionary with:
* `type` - `socket` or `unix_socket`
* `socket_type` - `stream`, `dgram`, `seqpacket`, or `raw`
* `local_ip` / `local_host` / `local_port` / `local_service` - local endpoint
* `remote_ip` / `remote_host` / `remote_port` / `remote_service` - remote endpoint

For Unix domain sockets:
* `path` - socket path

For regular files, returns a dictionary combining `stat` and `flags` results.

### flock

Acquire a file lock, execute a command, and release the lock.

```q
flock[lockfile;mode;cmd]
```

where:
* `lockfile` - path to the lock file (symbol or char vector)
* `mode` - `"w"` for exclusive write lock or `"r"` for shared read lock
* `cmd` - q expression to execute via `value`/`get` while holding the lock

Parent directories or the `lockfile` are automatically created if they don't exist.

Returns the result of `cmd`.

Please note, read-write locks allow multiple readers or a single writer, but never both simultaneously. Multiple threads can hold read locks concurrently, while write locks are exclusive to one thread and also block all readers.

### connect

Async TCP connection handler.

```q
connect[addr;cb]
```

where:
* `addr` - connection address in format `:host:port[:user:password]`
* `cb` - monadic callback function

The callback is called with either:
* The socket handle on successful connection
* An error string on failure

Returns `::` immediately (non-blocking).

Example:
```q
connect[`:localhost:5000;{$[-6h=type x;-1"Connected: ",string x;-2"Error: ",x]}]
```