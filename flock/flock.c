#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__)
#include <io.h>
#include <windows.h>
#include <direct.h>

#define LOCK_SH 1
#define LOCK_EX 2
#define LOCK_NB 4
#define LOCK_UN 8

#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif

static inline int flock(int fd, int operation) {
    HANDLE h = (HANDLE)_get_osfhandle(fd);
    if (h == INVALID_HANDLE_VALUE) {
        errno = EBADF;
        return -1;
    }
    OVERLAPPED overlapped = {0};
    DWORD flags = 0;
    if (operation & LOCK_NB) {
        flags |= LOCKFILE_FAIL_IMMEDIATELY;
    }
    if (operation & LOCK_EX) {
        flags |= LOCKFILE_EXCLUSIVE_LOCK;
    }
    if (operation & LOCK_UN) {
        if (UnlockFileEx(h, 0, 0xffffffff, 0xffffffff, &overlapped)) return 0;
    } else {
        if (LockFileEx(h, flags, 0, 0xffffffff, 0xffffffff, &overlapped)) return 0;
    }
    DWORD err = GetLastError();
    if (err == ERROR_LOCK_VIOLATION || err == ERROR_SHARING_VIOLATION) {
        errno = EWOULDBLOCK;
    } else {
        errno = EINVAL;
    }
    return -1;
}

#else
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define KXVER 3
#include "k.h"

#define PR __builtin_printf
#define STRNDUP  __builtin_strndup
#define STRRCHR  __builtin_strrchr
#define STRLEN   __builtin_strlen
#define STRNCPY  __builtin_strncpy
#define SNPRINTF __builtin_snprintf
#define MEMCPY   __builtin_memcpy

#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__)
#define mkdir(path, mode) _mkdir(path)
#endif

// Create directory and all parent directories
Z I mkdir_p(S path)
{
    S p = path;
    if(!path || !*path) R 0;

    C last = 0;
    for(S slash = path; *slash; slash++)
    {
        if(*slash=='/' || *slash=='\\')
        {
            last = *slash;
            *slash = 0;
            if(*p && mkdir(p, 0700) == -1 && errno != EEXIST)
            {
                *slash = last;
                R -1;
            }
            *slash = last;
        }
    }
    if(*p && mkdir(path, 0700) == -1 && errno != EEXIST)
        R -1;
    R 0;
}

K flock0(S lockfile, char mode, K cmd)
{
    if(':'==lockfile[0]) lockfile++;

    // Create parent directories if they don't exist
    C buff[4*4096]={0};
    STRNCPY(buff, lockfile, sizeof(buff)-1);
    S last_slash = STRRCHR(buff, '/');
    if(!last_slash) last_slash = STRRCHR(buff, '\\');
    if(last_slash)
    {
        *last_slash = 0;
        if(*buff && mkdir_p(buff) == -1)
            R orr("mkdir");
    }

    I h = open(lockfile,O_RDONLY|O_CREAT,0666);
    if(h==-1) R orr("open");
    I e = flock(h,(mode=='w'?LOCK_EX:LOCK_SH));
    if(e==-1)
    {
        R orr("flock");
    }
    K x=k(0,"get",r1(cmd),0);
    close(h);
    R x;
}

K qflock(K f, K mode, K cmd)
{
    if(mode->t!=-KC||(mode->g!='w'&&mode->g!='r')) R krr("mode");
    C buff[4*4096]={0};
    switch(f->t)
    {
        case -KS:
            R flock0(f->s,mode->g,cmd);
        case KC:
            if(f->n>sizeof(buff)-1) R krr("filename too long");
            STRNCPY(buff,f->s,f->n);
            R flock0(buff,mode->g,cmd);
        default: R krr("file");
    }
}

__attribute__((visibility("default")))
K kexport()
{
    R k(0,"(1#`flock)!",knk(1,dl(qflock,3)),0);
}