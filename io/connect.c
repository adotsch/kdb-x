#include "io.h"

typedef struct { 
    char* host;
    char* port;
    char* userpwd;
    K callback;
} ConnectionParams;

typedef struct {
    int sock;   // Socket handle or -1 if there's an error
    char* err;  // Error message
    char cap;   // Capability
    K callback;
} ConnectionResult;

Z int pipefd[2]={-1,-1};

// Function to connect to the host and port, return the connection handle or error message
Z void* connect_to_host(void* args)
{
    ConnectionParams* params=args;
    ConnectionResult result={.sock=-1, .err=0, .callback=params->callback};

    // Initialize the hints structure
    struct addrinfo hints={.ai_family=AF_UNSPEC,.ai_socktype=SOCK_STREAM}, *res, *p;
    int sock;

    // Get address info for the host
    if (getaddrinfo(params->host, params->port, &hints, &res) !=0)
    {
        result.err="getaddrinfo failed";
        goto exit;
    }

    // Try to connect using each returned addrinfo structure
    for (p=res; p !=NULL; p=p->ai_next)
    {
        sock=socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock==-1)
            continue;

        int flag=1;
        setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,&flag, sizeof(int));
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,&flag, sizeof(int));

        if (connect(sock, p->ai_addr, p->ai_addrlen)==-1)
        {
            close(sock);
            result.err="connection failed";
            continue;
            //break;
        }
        fcntl(sock, F_SETFD, FD_CLOEXEC);
        char buff[4096+1]={0};
        SNPRINTF(buff,4096,"%s\6",params->userpwd);
        if(sendto(sock,buff,STRLEN(buff)+1,0,NULL,0)==-1)
        {
            close(sock);
            result.err="handshake failed";
            break;
        }
        char c;
        if(recv(sock,&c,1,0)!=1 || (c!=3 && c!=6))
        {
            result.err="handshake failed";
            close(sock);
            break;
        }
        // Connection successful
        result.cap=c;
        result.sock=sock; 
        fcntl(sock, F_SETFL, O_RDONLY|O_NONBLOCK);
        break;
    }

    freeaddrinfo(res);

    if (result.sock==-1 && result.err==0)
       result.err="connection failed";

exit:
    free(params->host); free(params->port); free(params->userpwd); free(params);
    write(pipefd[1], &result, sizeof(result));

    pthread_exit(NULL);
}

Z K deliver_result(I h)
{
    ConnectionResult result;
    // Read the connection result from the pipe
    while(sizeof(result)==read(h,&result,sizeof(result)))
    {
        if(result.sock!=-1)
        {
            K sdq(I,...);
            sdq(result.sock,'c',result.cap,0);
            r0(k(0,"@",result.callback,ki(result.sock),0));
        }
        else
        {
            r0(k(0,"@",result.callback,kp(result.err),0));
        }
    }
    return 0;
}

Z K hopen(K addr,K cb)
{
    if(addr->t!=-KS) return krr("type");
    if(pipefd[0]==-1)
    {
        if(pipe2(pipefd,O_NONBLOCK)==-1) return orr("pipe creation failed");
        sd1(pipefd[0], deliver_result);
    }
    int col[4]={-1,-1,-1,-1}, cn=0;
    for(int i=0;addr->s[i] && cn<3; i++)
    {
        if(addr->s[i]==':') col[cn++]=i;
        if(addr->s[i]=='/') {cn=0;break;}
    }
    if(cn<2 || col[0]!=0) return krr("invalid host format");
    while(cn<4) col[cn++]=STRLEN(addr->s);
    ConnectionParams *params=malloc(sizeof(*params));
    *params = (ConnectionParams){
        col[1]>1 ? STRNDUP(addr->s+1, col[1]-1) : STRNDUP("localhost",9),
        STRNDUP(addr->s+col[1]+1,col[2]-col[1]-1),
        cn>2 ? STRNDUP(addr->s+col[2]+1,col[3]-col[2]-1) : STRNDUP("",0),
        r1(cb)};
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, connect_to_host, params) !=0)
        return orr("thread creation failed");
    pthread_detach(thread_id);
    return k(0,"::",0);
}