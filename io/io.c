
#include "info.c"
#include "lock.c"
#include "connect.c"
#include "pidw.c"
#include "udp.c"

__attribute__((visibility("default")))
K kexport()
{
    R k(0,"{"
            "`stat`flags`hinfo`flock`connect`pidw`spawn"
            "`udp_listen`udp_send"
        "!x}",
        knk(9,
            dl(qstat,1),dl(flags,1),dl(info,1),dl(qflock,3),dl(hopen,2),dl(pidw,2),dl(spawn,3),
            dl(udp_listen,2),dl(udp_send,3)
        ),0);
}