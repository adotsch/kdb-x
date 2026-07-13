# Advisory locks   for KDB

## Intro

This module provides file locking functionality for **KDB**, wrapping the advisory `flock` POSIX system call (and standard Win32 locking on Windows).

`flock` allows many readers or one writer to use the same resource at the same time.
 
## Synopsis

```q
q) // Load the module functions into the .q namespace
q) .q,:use`dot.flock
q) t:([]sym:1000000?.Q.a;price:1000000?100f)
q) \s      //4 threads
4
q) // Acquire an exclusive write lock on /tmp/lock while upserting to /tmp/tab/
q) {flock[`:/tmp/lock;"w";(`:/tmp/tab upsert;x)]} peach 1000 cut t;
q) // Without locking: this can fail or corrupt the table
q) {`:/tmp/tab2/ upsert x} peach 1000 cut t;
q) get`:/tmp/tab2
'length
q) count get`:/tmp/tab2/sym
9921000
q) count get`:/tmp/tab2/price
9784000
```

## Functions

### `flock`

Acquire a file lock, execute a command, and automatically release the lock.

- **Syntax**: `flock[lockfile; mode; cmd]`
- **Arguments**:
  - `lockfile`: symbol or string — path to the lock file. Any missing parent directories are automatically created.
  - `mode`: char — `"w"` for an exclusive write lock, or `"r"` for a shared read lock.
  - `cmd`: function / expression — the command to execute via `value`/`get` while holding the lock.
- **Returns**: The result of `cmd` on success, or throws a KDB error on failure.
- **Behavior**:
  - If another process or thread holds an exclusive lock (mode `"w"`), any other request for a read or write lock will block until the lock is released.
  - If another process holds a shared lock (mode `"r"`), requests for shared read locks will succeed immediately, but requests for exclusive write locks will block.
  - The lock is guaranteed to be released when `cmd` finishes executing.

Please note: The flock() is not universally supported by all filesystems. Whether it works depends on both the filesystem and the operating system's implementation. In particular, the user should be very careful when using it on network shares (like NFS, CIFS/SMB), as file locking behavior can be unreliable or unsupported.