CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

OBJ = src/network/client.c
OBJ += src/network/server.c
OBJ += src/misc/safe.c
OBJ += src/cryptosystem/rsa.c

all: main_test server client

main_test: tests/main_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o main.out

server: tests/network/server_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o server.out

client: tests/network/client_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o client.out

.PHONY: clean

clean:
	${RM} *.out