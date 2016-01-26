#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

typedef void (*closer) (int fd);

void connection_handler(int conn_fd, struct sockaddr_in *addr, closer cb)
{
    int pid = fork();
    if (pid > 0)
    {
        return;
    }
    fprintf(stdout, "Recevied connection from %s:%d, id: %d\n", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port), conn_fd);
    int i = 0;
    while(i < 10)
    {
        char buffer[256];
        memset(buffer, 0, 256);
        int new_bytes = 0;
        while(new_bytes == 0)
        {
            new_bytes = read(conn_fd, buffer, 256);
        }
        if(strcmp(buffer, "close") == 0)
        {
            break;
        }
        fprintf(stdout, "%d: %s\n", conn_fd, buffer);
        i = i + 1;
    }
    cb(conn_fd);
    exit(0);
}
