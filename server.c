#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>


#define SV_SOCK1_PATH "/tmp/socket1"
#define SV_SOCK2_PATH "/tmp/socket2"

#define BUF_SIZE 10


int main(void)
{
    int sfd1;
    struct sockaddr_un addr1, addr2, recvAddr;
    char buf[BUF_SIZE];
    socklen_t len;
    

    sfd1 = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sfd1)
    {
        perror("socket: ");
        return -1;
    }

    if (-1 == remove(SV_SOCK1_PATH) && ENOENT != errno)
    {
        perror("remove: ");
        return -1;
    }

    memset(&addr1, 0, sizeof(struct sockaddr_un));
    addr1.sun_family = AF_UNIX;
    strncpy(addr1.sun_path, SV_SOCK1_PATH, sizeof(addr1.sun_path) - 1);
    if (-1 == bind(sfd1, (struct sockaddr *)&addr1, sizeof(struct sockaddr_un)))
    {
        perror("bind: ");
        return -1;
    }

    // config socket2 address
    memset(&addr2, 0, sizeof(struct sockaddr_un));
    addr1.sun_family = AF_UNIX;
    strncpy(addr2.sun_path, SV_SOCK2_PATH, sizeof(addr2.sun_path) - 1);
    len = sizeof(struct sockaddr_un);
    // socket1 connect socket2
    if (-1 == connect(sfd1, (struct sockaddr *)&addr2, len))
    {
        perror("connect: ");
        return -1;
    }

    // socket2/3(client.c) sendto socket1
    for (;;)
    {
        if (-1 == recvfrom(sfd1, buf, BUF_SIZE, 0, (struct sockaddr *)&recvAddr, &len))
        {
            perror("recvfrom");
            return -1;
        }

        printf("recv: %s, from: %s\n", buf, recvAddr.sun_path);
        memset(buf, 0, sizeof(buf));
    }

    return 0;
}