all: server client

server: server.c connection.c
	gcc server.c connection.c -o server

client: client.c
	gcc client.c -o client
