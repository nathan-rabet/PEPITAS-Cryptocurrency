CC = gcc
CFLAGS = -I"headers" -I"tests" -Wall -Wextra -g -pthread

OBJ = src/network/client.c src/network/server.c src/misc/safe.c src/misc/bits_n.c

OBJ_TEST = tests/main_test.c tests/misc/bits_n_test.c

all: main_test server client

test: ${OBJ_TEST} ${OBJ}
	${CC} ${CFLAGS} $^ -o test

server: tests/network/server_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o server.out

client: tests/network/client_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o client.out

.PHONY: clean

clean:
	${RM} *.out test