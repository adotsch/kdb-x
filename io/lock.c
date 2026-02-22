#include "io.h"

#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__)
#define mkdir(path, mode) _mkdir(path)
#include <direct.h>
#else
#include <sys/stat.h>
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

K flock0(S lockfile, char mode, K cmd, I try)
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
    I e = flock(h,(mode=='w'?LOCK_EX:LOCK_SH)|(try?LOCK_NB:0));
    if(e==-1)
    {
        if(try && errno==EWOULDBLOCK)
        {
            close(h);
            R kb(0);
        }
        R orr("flock");
    }
    K x=k(0,"get",r1(cmd),0);
    close(h);
    R try?knk(2,kb(1),x):x;
}

K qflock(K f, K mode, K cmd)
{
    if(mode->t!=-KC||(mode->g!='w'&&mode->g!='r')) R krr("mode");
    C buff[4*4096]={0};
    switch(f->t)
    {
        case -KS:
            R flock0(f->s,mode->g,cmd,0);
        case KC:
            if(f->n>sizeof(buff)-1) R krr("filename too long");
            STRNCPY(buff,f->s,f->n);
            R flock0(buff,mode->g,cmd,0);
        default: R krr("file");
    }
}
