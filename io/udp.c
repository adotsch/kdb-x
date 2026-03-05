#include "k.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct {I h,p; K cb;} udp_cb;

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
        if(0>setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(yes)))
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
    return 0;
}

K udp_socket(K br)
{
    int sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sock<0) R orr("socket");
    if(br->t==-KB && br->g==1)
    {
        int yes = 1;
        if(0>setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(yes)))
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
                return kb(1);
            }
        }
        return kb(0);
    }
    R krr("nyi");
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
    ssize_t e = sendto(udp_sock, kC(msg), msg->n, 0,(struct sockaddr*)&addr,sizeof(addr));
    if(e<0) R orr("send");
    R r1(a);
}