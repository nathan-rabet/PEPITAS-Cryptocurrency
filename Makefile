CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

OBJ = src/network/client.c
OBJ += src/network/server.c

all: main_test

main_test: tests/main_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o main

server: tests/network/server_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o server

client: tests/network/client_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o client

.PHONY: clean

clean:
	${RM} main server client