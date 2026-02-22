#include "io.h"

K get_time(struct timespec t)
{
    R ktj(-KP,(J)(t.tv_sec-946684800)*1000000000LL+t.tv_nsec);
}

K stat_res(struct stat st)
{
    char* ty;
    switch(st.st_mode&S_IFMT)
    {
        case S_IFBLK:  ty="block_device"; break;
        case S_IFCHR:  ty="char_device"; break;
        case S_IFDIR:  ty="directory"; break;
        case S_IFIFO:  ty="fifo"; break;
        case S_IFLNK:  ty="symlink"; break;
        case S_IFREG:  ty="file"; break;
        case S_IFSOCK: ty="socket"; break;
        default:       ty="unknown"; break;
    }
    K k_=k(0,"`type`size`links`mode`inode`device`blksize`blocks`change`modify`access`uid`gid",0);
    K v_=knk(13,ks(ty),kj(st.st_size),kj(st.st_nlink),k(0,"8 8 8 vs",kj(st.st_mode&0777),0),kj(st.st_ino),kj(st.st_dev),kj(st.st_blksize),kj(st.st_blocks),
        get_time(st.st_ctim),get_time(st.st_mtim),get_time(st.st_atim),kj(st.st_uid),kj(st.st_gid));
    R xD(k_,v_);
}

K qstat(K x)
{
    struct stat st;
    int e;
    switch(xt)
    {
        case -KH: e=fstat(xh,&st); break;
        case -KI: e=fstat(xi,&st); break;
        case -KJ: e=fstat(xj,&st); break;
        case -KS: case KC:
        {
            char buff[4096]={0},*b;
            if(xt==-KS)
                b=xs+(xs[0]==':');
            else
            {
                b=buff;
                char* f = xC; J fn=xn<4095?xn:4095;
                if(':'==f[0]) f++,fn--;
                strncpy(buff,f,fn);
            }
            e=stat(b,&st);
        }
        break;
        default: R krr("type");
    }
    if(e==-1) R orr("stat");
    R stat_res(st);
}

K flag_res(I f)
{
    K x=ktn(KS,5);xn=0;
    if((f&O_ACCMODE)!=O_WRONLY)     xS[xn++]=ss("read");
    if((f&O_ACCMODE)!=O_RDONLY)     xS[xn++]=ss("write");
    if(f&O_APPEND)                  xS[xn++]=ss("append");
    if(f&O_NONBLOCK)                xS[xn++]=ss("nonblock");
    if((f&O_SYNC)==O_SYNC)          xS[xn++]=ss("sync");
    else if((f&O_DSYNC)==O_DSYNC)   xS[xn++]=ss("dsync");
    R x;
}

K flags(K x)
{
    I h;
    switch(xt)
    {
        case -KH: h=xh; break;
        case -KI: h=xi; break;
        case -KJ: h=xj; break;
        default: R krr("type");
    }
    I f=fcntl(h,F_GETFL);
    if(f==-1) R orr("fcntl");
    R flag_res(f);
}

K info(K x)
{
    I h;
    C buf[INET6_ADDRSTRLEN]={0}, host[1024]={0}, serv[1024]={0};
    switch(xt)
    {
        case -KH: h=xh; break; 
        case -KI: h=xi; break; 
        case -KJ: h=xj; break; 
        default: R krr("type");
    }
    struct stat st;
    if(fstat(h,&st) == -1) R orr("fstat");
    I flags = fcntl(h,F_GETFL);
    if(flags==-1) R orr("fcntl");
    if(S_ISSOCK(st.st_mode))
    {
        struct sockaddr_storage addr; socklen_t addrlen=sizeof(addr);
        struct sockaddr* aptr=(struct sockaddr*)&addr;
        K v_=ktn(0,10); v_->n=1;
        int so_type; socklen_t optlen=sizeof(so_type);
        if(getsockopt(h,SOL_SOCKET,SO_TYPE,&so_type,&optlen)<0) R r0(v_),orr("getsockopt");
        kK(v_)[v_->n++] = ks(so_type==SOCK_STREAM?"stream":so_type==SOCK_DGRAM?"dgram":so_type==SOCK_SEQPACKET?"seqpacket":"raw");
        for(int w=0;w<2;w++)
        {
            if(w)
            {
                if(getpeername(h,aptr,&addrlen)<0) R r0(v_),orr("getpeername");
            }
            else
            {
                if(getsockname(h,aptr,&addrlen)<0) R r0(v_),orr("getsockname");
            }
            if(getnameinfo(aptr,addrlen,host,sizeof(host),serv,sizeof(serv),0)<0) R r0(v_),orr("getnameinfo");
            switch(aptr->sa_family)
            {
                case AF_INET:
                {
                    struct sockaddr_in* a=(struct sockaddr_in*)aptr;
                    inet_ntop(AF_INET,&a->sin_addr,buf,sizeof(buf));
                    kK(v_)[v_->n++] = kp(buf);
                    kK(v_)[v_->n++] = kp(host);
                    kK(v_)[v_->n++] = ki(ntohs(a->sin_port));
                    kK(v_)[v_->n++] = kp(serv);
                }
                break;
                case AF_INET6:
                {
                    struct sockaddr_in6* a6=(struct sockaddr_in6*)aptr;
                    inet_ntop(AF_INET6,&a6->sin6_addr,buf,sizeof(buf));
                    kK(v_)[v_->n++] = kp(buf);
                    kK(v_)[v_->n++] = kp(host);
                    kK(v_)[v_->n++] = ki(ntohs(a6->sin6_port));
                    kK(v_)[v_->n++] = kp(serv);
                }
                break;
                case AF_UNIX:
                {
                    struct sockaddr_un *sun = (struct sockaddr_un *)aptr;
                    if(v_->n==2) kK(v_)[v_->n++] = kp(sun->sun_path+1); //skip leading null byte
                }
                break;
                default:
                    R r0(v_),krr("unsupported socket address family");
            }
        }
        K k_;
        if(v_->n==3)
        {
            k_ = k(0,"`type`socker_type`path",0);
            kK(v_)[0]=ks("unix_socket");
        }
        else
        {
            k_ = k(0,"`type`socker_type`local_ip`local_host`local_port`local_service`remote_ip`remote_host`remote_port`remote_service",0);
            kK(v_)[0]=ks("socket"); 
        }
        R xD(k_,v_);
    }
    R k(0,"{(`type`flags!(first x;y)),1_x}",stat_res(st),flag_res(flags),0);
}
