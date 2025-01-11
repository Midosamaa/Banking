CC = gcc
CFLAGS = -Wall -pthread
SRC = server_tcp.c client_tcp.c comptes.c
OBJ = $(SRC:.c=.o)
EXEC = server_tcp client_tcp

all: $(EXEC)

server_tcp: server_tcp.o comptes.o
	$(CC) $(CFLAGS) -o server_tcp server_tcp.o comptes.o

client_tcp: client_tcp.o comptes.o
	$(CC) $(CFLAGS) -o client_tcp client_tcp.o comptes.o

clean:
	rm -f $(OBJ) $(EXEC)
