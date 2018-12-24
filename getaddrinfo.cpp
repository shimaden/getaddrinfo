#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <stdio.h>

static const char *family_name(int family)
{
    switch(family)
    {
        case 2:
            return "AF_INET";
        case 10:
            return "AF_INET6";
        default:
            return "unknown";
    }
}

static const char *sockettype_name(int sockettype)
{
    switch(sockettype)
    {
        case SOCK_STREAM:
            return "SOCK_STREAM";
        case SOCK_DGRAM:
            return "SOCK_DGRAM";
        default:
            return "unknown";
    }
}

static const char *proto_name(int protocol)
{
    switch(protocol)
    {
        case 0:
            return "ANY";
        case IPPROTO_TCP:
            return "IPPROTO_TCP";
        case IPPROTO_UDP:
            return "IPPROTO_UDP";
        default:
            return "unknown";
    }
}

static void print_ip6(const sockaddr_in6 *sin6)
{
    char addr_s[INET6_ADDRSTRLEN];
    printf("sin_family: %2d %s\n", sin6->sin6_family, family_name(sin6->sin6_family));
    printf("sin_port  : %d\n", sin6->sin6_port);
    printf("sin_addr  : %s\n",
        inet_ntop(AF_INET6, &sin6->sin6_addr.s6_addr, addr_s, sizeof addr_s)
    );
}

static void print_ip4(const sockaddr_in *sin)
{
    char addr_s[INET_ADDRSTRLEN];
    printf("sin_family: %2d %s\n", sin->sin_family, family_name(sin->sin_family));
    printf("sin_port  : %d\n", sin->sin_port);
    printf("sin_addr  : %s\n",
        inet_ntop(AF_INET, &sin->sin_addr.s_addr, addr_s, sizeof addr_s)
    );
}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "%s <domain>\n", argv[0]);
        return 1;
    }
    //const char *nodename = "www.google.co.jp";
    const char *nodename = argv[1];
    const char *servname = NULL;
    addrinfo hints;
    addrinfo *res;

    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = PF_UNSPEC; //AF_INET | AF_INET6;
    hints.ai_socktype = 0;
    hints.ai_protocol = 0;

    int result = getaddrinfo(nodename, servname, &hints, &res);
    if(result != 0)
    {
        fprintf(stderr, "%s\n", gai_strerror(result));
        return 1;
    }

    for(const addrinfo *r = res ; r->ai_next != NULL ; r = r->ai_next)
    {
        printf("Flags     : %04X\n"
               "Family    : %2d %s\n"
               "Sock type : %2d %s\n"
               "Protocol  : %2d %s\n",
                r->ai_flags,
                r->ai_family, family_name(r->ai_family),
                r->ai_socktype, sockettype_name(r->ai_socktype),
                r->ai_socktype, proto_name(r->ai_protocol)
        );
        printf("Addr len  : %u\n", r->ai_addrlen);
        printf("Canon name: %s\n", r->ai_canonname);

        const sockaddr_storage *s = (sockaddr_storage *)r->ai_addr;
        printf("SS Family : %2d %s\n", s->ss_family, family_name(s->ss_family));

        if(r->ai_family == AF_INET)
        {
            print_ip4((sockaddr_in *)s);
        }
        else if(r->ai_family == AF_INET6)
        {
            print_ip6((sockaddr_in6 *)s);
        }

        printf("\n");

    }

    freeaddrinfo(res);

    return 0;
}
