#define _GNU_SOURCE
#define PR __builtin_printf
#define STRNDUP  __builtin_strndup
#define STRRCHR  __builtin_strrchr
#define STRLEN   __builtin_strlen
#define STRNCPY  __builtin_strncpy
#define SNPRINTF __builtin_snprintf
#define MEMCPY   __builtin_memcpy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#ifndef socklen_t
typedef int socklen_t;
#endif
#define close closesocket
#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>
#endif

#define KXVER 3
#include "k.h"

typedef struct {I h,p; K cb;} udp_cb;

#ifdef _WIN32
Z void udp_init(void) __attribute__((constructor));
Z void udp_init(void) { WSADATA wsaData; WSAStartup(MAKEWORD(2,2), &wsaData); }
#endif

Z I udp_sock = -1;
Z I n_udp_cbs = 0;
Z udp_cb* udp_cbs = 0;

Z I mk_udp_socket(I broadcast)
{
    I sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sock<0) R orr("socket"),-1;
    if(broadcast)
    {
        I yes = 1;
        if(0>setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(const char*)&yes,sizeof(yes)))
            R orr("broadcast"),close(sock),-1;
    }
    R sock;
}

K udp_recv(I h)
{
    char buf[65536];
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    ssize_t len = recvfrom(h,buf,sizeof(buf)-1,0,(struct sockaddr*)&sender,&sender_len);
    if(len)
    {
        K in_addr = ks(inet_ntoa(sender.sin_addr));
        K msg = ktn(KG,len); MEMCPY(kC(msg),buf,len);
        for(int i=0;i<n_udp_cbs;i++)
            if(udp_cbs[i].h==h)
            {
                K e = k(0,".",r1(udp_cbs[i].cb),knk(2,in_addr,msg),0);
                if(e->t==-128)
                {
                    PR("udp message error on socket %i: %s\n",h,e->s);
                }
                r0(e);
            }
    }
    R 0;
}

K udp_socket(K br)
{
    int sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sock<0) R orr("socket");
    if(br->t==-KB && br->g==1)
    {
        int yes = 1;
        if(0>setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(const char*)&yes,sizeof(yes)))
            R orr("broadcast");
    }
    R ki(sock);
}

K udp_listen(K p, K cb)
{
    if(p->t!=-KI && p->t!=-KJ) R krr("type");
    if(p->i>0)  //start listening
    {
        int sock = mk_udp_socket(0);
        if(sock<0) R 0;
        struct sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(p->i);
        addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(sock,(struct sockaddr*)&addr,sizeof(addr))<0) R orr("bind"),close(sock),(K)0;
        sd1(sock,udp_recv);
        for(int i=0;i<n_udp_cbs;i++)                                //reuse previous slot
        {
            if(udp_cbs[i].h==sock)
            {
                K x = udp_cbs[i].cb;
                udp_cbs[i].p = p->i;
                udp_cbs[i].cb = r1(cb);
                r0(x);
                R ki(sock);
            }
        }
        udp_cbs = realloc(udp_cbs,sizeof(udp_cb)*(n_udp_cbs+1));    //create new slot
        udp_cbs[n_udp_cbs++] = (udp_cb){sock,p->i,r1(cb)};
        R ki(sock);
    }
    if(p->i<0)  //stop listening
    {
        I port = -p->i;
        for(int i=0;i<n_udp_cbs;i++)
        {
            if(udp_cbs[i].p==port)
            {
                r0(udp_cbs[i].cb);
                sd0(udp_cbs[i].h);
                if(i<n_udp_cbs-1)
                {
                    udp_cbs[i] = udp_cbs[n_udp_cbs-1];
                }
                n_udp_cbs--;    //We avoid unessesary bureaucracy and won't reallocate 
                R kb(1);
            }
        }
        R kb(0);
    }
    R krr("nyi");
}

K ujoin(K h, K a)
{
    if((h->t!=-KI && h->t!=-KJ) || a->t!=-KS) R krr("type");
    int sock = h->i;
    if(sock==0) R krr("socket");
    struct ip_mreq mreq = {0};
    mreq.imr_multiaddr.s_addr = inet_addr(a->s);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if(mreq.imr_multiaddr.s_addr == INADDR_NONE) R krr("address");
    if(sock>0)
    {
        if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&mreq, sizeof(mreq))<0)
            R orr("join");
    }
    else
    {
        if(setsockopt(-sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char *)&mreq, sizeof(mreq))<0)
            R orr("leave");
    }
    R r1(h);
}

K udp_send(K a, K p, K msg)
{
    if(udp_sock<0)
    {
        I sock = mk_udp_socket(1);
        if(sock<0) R 0;
        udp_sock = sock;
    }
    if(a->t!=-KS) krr("type");
    if(p->t!=-KI && p->t!=-KJ) R krr("type");
    if(msg->t!=KC && msg->t!=KG) R krr("type");
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(p->i);
    addr.sin_addr.s_addr = inet_addr(a->s);
    ssize_t e = sendto(udp_sock, (const char *)kC(msg), msg->n, 0,(struct sockaddr*)&addr,sizeof(addr));
    if(e<0) R orr("send");
    R r1(a);
}

K interfaces(K unused)
{
    K t = k(0,"1!([]interface:0#`;address:`;netmask:`;broadcast:`)",0);
#if defined(__linux__) || defined(__APPLE__)
    struct ifaddrs *ifaddr, *ifa;
    char addr[INET_ADDRSTRLEN];
    char mask[INET_ADDRSTRLEN];
    char bcast[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        R r0(t),orr("getifaddrs");
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        bcast[0] = 0;
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family != AF_INET)
            continue;   // IPv4 only

        struct sockaddr_in *ip =
            (struct sockaddr_in *)ifa->ifa_addr;
        struct sockaddr_in *netmask =
            (struct sockaddr_in *)ifa->ifa_netmask;
        struct sockaddr_in *broad =
            (struct sockaddr_in *)ifa->ifa_broadaddr;

        inet_ntop(AF_INET, &ip->sin_addr, addr, sizeof(addr));
        inet_ntop(AF_INET, &netmask->sin_addr, mask, sizeof(mask));

        if ((ifa->ifa_flags & IFF_BROADCAST) && broad != NULL) {
            inet_ntop(AF_INET, &broad->sin_addr, bcast, sizeof(bcast));
        }
        t = k(0,"upsert",t,knk(4,ks(ifa->ifa_name),ks(addr),ks(mask),ks(bcast)),0);
    }

    freeifaddrs(ifaddr);
#elif defined(_WIN32)
    PIP_ADAPTER_INFO pAdapterInfo = NULL;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        R t;
    }
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            R t;
        }
    }
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR) {
        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
        while (pAdapter != NULL) {
            PIP_ADDR_STRING pIpAddr = &pAdapter->IpAddressList;
            while (pIpAddr != NULL) {
                unsigned long ip = inet_addr(pIpAddr->IpAddress.String);
                unsigned long mask = inet_addr(pIpAddr->IpMask.String);
                if (ip != 0 && ip != INADDR_NONE) {
                    unsigned long bcast = ip | ~mask;
                    struct in_addr bcast_addr;
                    bcast_addr.s_addr = bcast;
                    char *bcast_str = inet_ntoa(bcast_addr);
                    t = k(0,"upsert",t,knk(4,ks(pAdapter->Description),ks(pIpAddr->IpAddress.String),ks(pIpAddr->IpMask.String),ks(bcast_str)),0);
                }
                pIpAddr = pIpAddr->Next;
            }
            pAdapter = pAdapter->Next;
        }
    }
    if (pAdapterInfo) {
        free(pAdapterInfo);
    }
#endif
    R t;
}

__attribute__((visibility("default")))
K kexport()
{
    R k(0,"`ulisten`ujoin`usend`ifls!",knk(4,dl(udp_listen,2),dl(ujoin,2),dl(udp_send,3),dl(interfaces,1)),0);
}