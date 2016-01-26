#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main()
{
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in *addr;
    addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

    addr->sin_family = AF_INET;
    addr->sin_port = htons(9999);
    int dest_ret = inet_aton("127.0.0.1", &(addr->sin_addr));
    if (dest_ret == 0)
    {
        fprintf(stderr, "Error with destination address: %s\n", strerror(errno));
        return 1;
    }

    int conn_ret = connect(sock_fd, (struct sockaddr *) addr, sizeof(struct sockaddr_in));
    if (conn_ret != 0)
    {
        fprintf(stderr, "Error with connecting to address: %s\n", strerror(errno));
        return 1;
    }
    while(1)
    {
        char buffer[256];
        scanf("%s", buffer);
        int n = write(sock_fd, buffer, strlen(buffer));
        if (n < 0)
        {
            fprintf(stderr, "Error writing to socket: %s\n", strerror(errno));
            return 1;
        }
        if (strcmp("close", buffer) == 0)
        {
            break;
        }
        memset(buffer, 0, 256);
    }
    return 0;
}

