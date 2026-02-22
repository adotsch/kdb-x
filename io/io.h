#ifndef IO_H
#define IO_H

#define KXVER 3
#include "k.h"

#define _GNU_SOURCE
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <netdb.h>
#include "k.h"

#define PR __builtin_printf
#define STRNDUP  __builtin_strndup
#define STRRCHR  __builtin_strrchr
#define STRLEN   __builtin_strlen
#define STRNCPY  __builtin_strncpy
#define SNPRINTF __builtin_snprintf
#endif