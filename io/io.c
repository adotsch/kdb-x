
#include "info.c"
#include "lock.c"
#include "connect.c"

__attribute__((visibility("default")))
K kexport()
{
    R k(0,"{`stat`flags`hinfo`flock`connect!x}",knk(5,dl(qstat,1),dl(flags,1),dl(info,1),dl(qflock,3),dl(hopen,2)),0);
}