#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 100

int main(void)
{
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];
    struct sockaddr_un addr;

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sfd)
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(&addr.sun_path[1], "xyz", sizeof(addr.sun_path) - 2);

    if (-1 == connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)))
    {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    while (numRead = read(STDIN_FILENO, buf, BUF_SIZE))
    {
        if (numRead != write(sfd, buf, numRead))
        {
            perror("write error");
            exit(EXIT_FAILURE);
        }
    }
    
    if (-1 == numRead)
    {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}