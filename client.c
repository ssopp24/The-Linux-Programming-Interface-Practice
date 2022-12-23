#include <string.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#define SV_SOCK_PATH "/tmp/socket"

    int
    main(void)
{
    int sfd, i=0;
    struct sockaddr_un addr;
    socklen_t len;
    ssize_t numBytes;

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sfd)
    {
        printf("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);
    char str[10000];
    memset(&str, 0, sizeof(str));

    for (;;)
    {
        sprintf(str, "%d", i++);
        printf("准备发送\n");
        numBytes = sendto(sfd, str, strlen(str), 0, (struct sockaddr *)&addr, sizeof(struct sockaddr));
        if (strlen(str) != numBytes)
        {
            perror("sendto: ");
            return -1;
        }
        printf("发送成功: %s\n", str);
        memset(&str, 0, sizeof(str));
    }

    return 0;
}