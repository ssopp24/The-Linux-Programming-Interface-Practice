#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#define BACKLOG 5
#define BUF_SIZE 100

int main(void)
{
   int sfd, cfd;
   ssize_t numRead;
   struct sockaddr_un addr;
   char buf[BUF_SIZE];

   sfd = socket(AF_UNIX, SOCK_STREAM, 0);
   if (-1 == sfd)
   {
      perror("socket error");
      exit(EXIT_FAILURE);
   }

   memset(&addr, 0, sizeof(struct sockaddr_un));
   addr.sun_family = AF_UNIX;
   strncpy(&addr.sun_path[1], "xyz", sizeof(addr.sun_path) - 2);

   if (-1 == bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)))
   {
      perror("bind error");
      exit(EXIT_FAILURE);
   }

   if (-1 == listen(sfd, BACKLOG))
   {
      perror("listen error");
      exit(EXIT_FAILURE);
   }

   for (;;)
   {
      cfd = accept(sfd, NULL, NULL);
      if (-1 == cfd)
      {
         perror("accept error");
         exit(EXIT_FAILURE);
      }

      while ((numRead = read(cfd, buf, BUF_SIZE) > 0))
      {
         if (write(STDOUT_FILENO, buf, numRead) != numRead)
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

      if (-1 == close(cfd))
      {
         perror("close error");
         exit(EXIT_FAILURE);
      }
   }

   exit(EXIT_SUCCESS);
}