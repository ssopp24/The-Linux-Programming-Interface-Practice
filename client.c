#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>


#define SV_SOCK1_PATH "/tmp/socket1"
#define SV_SOCK2_PATH "/tmp/socket2"
#define SV_SOCK3_PATH "/tmp/socket3"

int main(void)
{
    int sfd2, sfd3;
    struct sockaddr_un addr2, addr3, sendAddr;
    socklen_t len;

    sfd2 = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sfd2)
    {
        perror("socket: ");
        return -1;
    }

    if (-1 == remove(SV_SOCK2_PATH) && ENOENT != errno)
    {
        perror("remove: ");
        return -1;
    }
    if (-1 == remove(SV_SOCK3_PATH) && ENOENT != errno)
    {
        perror("remove: ");
        return -1;
    }

    memset(&addr2, 0, sizeof(struct sockaddr_un));
    addr2.sun_family = AF_UNIX;
    strncpy(addr2.sun_path, SV_SOCK2_PATH, sizeof(addr2.sun_path) - 1);
    if (-1 == bind(sfd2, (struct sockaddr *)&addr2, sizeof(struct sockaddr_un)))
    {
        perror("bind: ");
        return -1;
    }

    sfd3 = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sfd3)
    {
        perror("socket: ");
        return -1;
    }

    memset(&addr3, 0, sizeof(struct sockaddr_un));
    addr3.sun_family = AF_UNIX;
    strncpy(addr3.sun_path, SV_SOCK3_PATH, sizeof(addr3.sun_path) - 1);
    if (-1 == bind(sfd3, (struct sockaddr *)&addr3, sizeof(struct sockaddr_un)))
    {
        perror("bind: ");
        return -1;
    }

    len = sizeof(struct sockaddr_un);
    memset(&sendAddr, 0, sizeof(struct sockaddr_un));
    sendAddr.sun_family = AF_UNIX;
    strncpy(sendAddr.sun_path, SV_SOCK1_PATH, sizeof(sendAddr.sun_path) - 1);
    if (-1 == sendto(sfd2, "abc", 3, 0, (struct sockaddr *)&sendAddr, len)){
        perror("sendto: ");
    }

    if (-1 == sendto(sfd3, "123", 3, 0, (struct sockaddr *)&sendAddr, len))
    {
        perror("sendto: ");
    }

    return 0;
}