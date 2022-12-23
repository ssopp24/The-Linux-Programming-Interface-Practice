#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#define SV_SOCK_PATH "/tmp/socket"
#define BUF_SIZE 4

int main(void)
{
    int sfd;
    struct sockaddr_un addr;
    socklen_t len;
    char buf[BUF_SIZE];
    ssize_t numBytes;

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sfd)
    {
        printf("socket");
        return -1;
    }

    if (-1 == remove(SV_SOCK_PATH) && ENOENT != errno)
    {
        printf("remove");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (-1 == bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)))
    {
        printf("bind");
        return -1;
    }

    for (;;){
        printf("准备接收\n");
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, NULL, NULL);
        if (-1 == numBytes){
            perror("recvfrom: ");
            return -1;
        }
        printf("接收成功: %ld, %s\n", (long)numBytes, buf);
        memset(&buf, 0, sizeof(buf));
        printf("sleep\n");
        sleep(1);
        printf("sleep over\n");
    }

    return 0;
}