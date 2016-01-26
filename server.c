#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define MAX_PROCESSES 10

void closer(int fd)
{
    close(fd);
}

int main()
{
    int curr_threads = 0;
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)
    {
        fprintf(stderr, "Error while creating socket; err: %s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in *sa_addr;
    sa_addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

    sa_addr->sin_family = AF_INET;
    sa_addr->sin_port = htons(9999);
    int addr = inet_aton("127.0.0.1", &(sa_addr->sin_addr));

    if (addr == 0)
    {
        fprintf(stderr, "Invalid address: %s\n", "127.0.0.1");
        return 1;
    }

    int bind_ret = bind(sock_fd, (struct sockaddr *) sa_addr, sizeof(struct sockaddr_in));
    if (bind_ret != 0)
    {
        fprintf(stderr, "Error while binding; err: %s\n", strerror(errno));
        return 1;
    }

    int listen_ret = listen(sock_fd, SOMAXCONN);
    if (listen_ret != 0)
    {
        fprintf(stderr, "Error while listening on socket: %s\n", strerror(errno));
        return 1;
    }
    while(1)
    {
        if (curr_threads == MAX_PROCESSES)
        {
            int pid;
            int finished_threads = 0;
            while(pid = waitpid(-1, NULL, 0))
            {
                // callback approach might be better for maintaing count
                // also this block can cause indefinite waits, as we wait for 
                // existing child processes to exit
                finished_threads = finished_threads + 1;
                if (errno = ECHILD)
                {
                    break;
                }
            }
            curr_threads = curr_threads - finished_threads;
        }
        fprintf(stdout, "waiting for connection...\n");
        struct sockaddr_in *addr;
        addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
        socklen_t *addr_len = (socklen_t *) malloc(sizeof(socklen_t));
        int conn_fd = accept(sock_fd, (struct sockaddr *) addr, addr_len);
        if (conn_fd < 0)
        {
            fprintf(stderr, "Error accepting connections: %s\n", strerror(errno));
            return 1;
        }
        curr_threads = curr_threads + 1;
        connection_handler(conn_fd, addr, closer);
    }
    return 0;
}
